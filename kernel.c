// kernel.c
#include <stdint.h>
#include "gdt.h"
#include "idt.h"
#include "serial.h"

void kernel_main(uint32_t multiboot_magic, uint32_t multiboot_info)
{
    (void)multiboot_magic;
    (void)multiboot_info;

    gdt_init();
    idt_init();
    serial_init();

    serial_write_string("Minimal kernel started via GRUB.\n");
    serial_write_string("GDT/IDT initialized, serial ready.\n");

    while (1) {
        __asm__ volatile ("hlt");
    }
}
