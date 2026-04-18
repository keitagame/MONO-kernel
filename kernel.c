// kernel.c
#include <stdint.h>
#include "gdt.h"
#include "idt.h"
#include "serial.h"
#include "multiboot.h"
#include "pmm.h"
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline void pic_disable(void)
{
    outb(0x21, 0xFF); // master PIC mask all
    outb(0xA1, 0xFF); // slave PIC mask all
}
void kernel_main(uint32_t multiboot_magic, uint32_t multiboot_info)
{
    struct multiboot_info* mb = (struct multiboot_info*)multiboot_info;
    (void)multiboot_magic;
    (void)multiboot_info;
    pic_disable();
    
    gdt_init();
    idt_init();
    serial_init();

    serial_write_string("Minimal kernel started via GRUB.\n");
    serial_write_string("GDT/IDT initialized, serial ready.\n");
    pmm_init(mb);
    serial_write_string("PMM initialized.\n");

    void* page = pmm_alloc_page();
    serial_write_string("Allocated page at: ");
    
    while (1) {
        __asm__ volatile ("hlt");
    }
}
