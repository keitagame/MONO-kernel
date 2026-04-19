// kernel.c
#include <stdint.h>
#include "gdt.h"
#include "idt.h"
#include "serial.h"
#include "multiboot.h"
#include "process.h"
#include "pmm.h"
#include "timer.h"
#include "tss.h"
#include "initrd.h"
//#include "usermode.h"
void serial_write_hex(uint32_t val)
{
    const char* hex = "0123456789ABCDEF";
    char buf[9];
    buf[8] = '\0';

    for (int i = 7; i >= 0; i--) {
        buf[i] = hex[val & 0xF];
        val >>= 4;
    }

    serial_write_string(buf);
}
void serial_write_dec(uint32_t val)
{
    char buf[11];
    int i = 0;

    if (val == 0) {
        serial_write_char('0');
        return;
    }

    while (val > 0) {
        buf[i++] = '0' + (val % 10);
        val /= 10;
    }

    // 逆順に出力
    while (i--) {
        serial_write_char(buf[i]);
    }
}


initrd_file_t initrd_find(uint8_t* initrd_start, uint32_t initrd_size, const char* path);
#include "sys.h"
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline void pic_disable(void)
{
    outb(0x21, 0xFF); // master PIC mask all
    outb(0xA1, 0xFF); // slave PIC mask all
}
process_t* scheduler_next()
{
    int start = current ? (current - proc_table + 1) : 0;

    for (int i = 0; i < MAX_PROC; i++) {
        int idx = (start + i) % MAX_PROC;
        if (proc_table[idx].state == PROC_READY)
            return &proc_table[idx];
    }
    return current; // 他にいなければ同じ
}
void task1() {
    const char msg[] = "hello from task1\n";
    while (1) {
        
        //serial_write_string("");
        sys_write(1, msg, sizeof(msg) - 1);
        for (volatile int i = 0; i < 1000000; i++);
    }
}


/*void timer_handler()
{
    process_t* next = scheduler_next();
    if (next != current) {
        process_t* old = current;
        current = next;
        switch_to(old->tf, next->tf);
    }
}*/
void timer_handler()
{
    // 何もしない
}

/*void timer_handler(void)
{
    static int ticks = 0;
    ticks++;
    if (ticks % 100 == 0) {
        serial_write_string("tick\n");
    }
    // ★ ここでは scheduler_next() や switch_to() を呼ばない
}*/
extern void user_main();
void kernel_main(uint32_t multiboot_magic, uint32_t multiboot_info)
{
    struct multiboot_info* mb = (struct multiboot_info*)multiboot_info;
    (void)multiboot_magic;
    (void)multiboot_info;
    //pic_disable();
    if (!(mb->flags & (1 << 3))) {
    serial_write_string("no modules flag in multiboot info\n");
} else if (mb->mods_count == 0) {
    serial_write_string("mods_count == 0 (no initrd)\n");
}else{
    multiboot_module_t* mod = (multiboot_module_t*) mb->mods_addr;

    uint8_t* initrd_start = (uint8_t*) mod->mod_start;
    uint8_t* initrd_end   = (uint8_t*) mod->mod_end;
    uint32_t initrd_size  = initrd_end - initrd_start;

    serial_write_string("initrd loaded.\n");

    gdt_init();
    idt_init();
    serial_init();

    serial_write_string("Minimal kernel started via GRUB.\n");
    serial_write_string("GDT/IDT initialized, serial ready.\n");
    pmm_init(mb);
    serial_write_string("PMM initialized.\n");

    void* page = pmm_alloc_page();
    serial_write_string("Allocated page at: ");
    initrd_file_t init = initrd_find(initrd_start, initrd_size, "/init");
    
serial_write_string("initrd_start = 0x");
serial_write_hex(initrd_start);
serial_write_string("\n");

serial_write_string("initrd_end   = 0x");
serial_write_hex(initrd_end);
serial_write_string("\n");

serial_write_string("initrd_size  = ");
serial_write_dec(initrd_size);
serial_write_string(" bytes\n");
    if (!init.data) {
        serial_write_string("initrd: /init not found\n");
        for(;;) __asm__ volatile("hlt");
    }
    //timer_init(100); // 100Hz
    //asm volatile("sti");
    //process_create(task1);
    //process_create(task2);
    tss_init();
    tss_set_kernel_stack(0x00800000);
    serial_write_string("Kernel OK\n");
    uint32_t user_stack = 0x800000; // 適当な空き領域
    enter_user_mode((uint32_t)user_main, user_stack);
    //current = scheduler_next();
    //switch_to(0, current->tf);
    while (1) {
        __asm__ volatile ("hlt");
    }
}
}
