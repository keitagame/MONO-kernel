// idt.c
#include "idt.h"

static struct idt_entry idt[256];
static struct idt_ptr   idtp;

extern void irq0_handler();

static inline void lidt(void* base, uint16_t size)
{
    struct {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) idtr = { size - 1, (uint32_t)base };

    __asm__ volatile ("lidt %0" : : "m"(idtr));
}

void idt_set_gate(int n, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt[n].base_low  = handler & 0xFFFF;
    idt[n].base_high = (handler >> 16) & 0xFFFF;

    idt[n].sel       = sel;     // 例: 0x08 (カーネルコードセグメント)
    idt[n].always0   = 0;
    idt[n].flags     = flags;   // 例: 0x8E (present, ring0, 32bit interrupt gate)
}
extern void isr_syscall_stub();
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
    idt_set_gate(32, (uint32_t)irq0_handler, 0x08, 0x8E);
    

    idt_set_gate(0x80, (uint32_t)isr_syscall_stub, 0x08, 0xEE);
    lidt(idt, sizeof(idt));
}
