// kernel.c
#include <stdint.h>
#include "gdt.h"
#include "idt.h"
#include "serial.h"
#include "multiboot.h"
#include "process.h"
#include "pmm.h"
#include "timer.h"

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
    
    gdt_init();
    idt_init();
    serial_init();

    serial_write_string("Minimal kernel started via GRUB.\n");
    serial_write_string("GDT/IDT initialized, serial ready.\n");
    pmm_init(mb);
    serial_write_string("PMM initialized.\n");

    void* page = pmm_alloc_page();
    serial_write_string("Allocated page at: ");
    
    //timer_init(100); // 100Hz
    //asm volatile("sti");
    //process_create(task1);
    //process_create(task2);
    serial_write_string("Kernel OK\n");
    uint32_t user_stack = 0x800000; // 適当な空き領域
    enter_user_mode((uint32_t)user_main, user_stack);
    //current = scheduler_next();
    //switch_to(0, current->tf);
    while (1) {
        __asm__ volatile ("hlt");
    }
}
