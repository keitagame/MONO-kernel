// idt.c
#include "idt.h"

static struct idt_entry idt[256];
static struct idt_ptr   idtp;

static inline void lidt(void* base, uint16_t size)
{
    struct {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) idtr = { size - 1, (uint32_t)base };

    __asm__ volatile ("lidt %0" : : "m"(idtr));
}

void idt_init(void)
{
    for (int i = 0; i < 256; i++) {
        idt[i].base_low = 0;
        idt[i].sel = 0x08;      // code segment
        idt[i].always0 = 0;
        idt[i].flags = 0x8E;    // present, ring0, 32bit interrupt gate
        idt[i].base_high = 0;
    }

    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;

    lidt(idt, sizeof(idt));
}
