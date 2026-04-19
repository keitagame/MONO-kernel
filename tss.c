// tss.c
#include <stdint.h>
#include "gdt.h"

struct tss_entry {
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1, ss1;
    uint32_t esp2, ss2;
    uint32_t cr3;
    uint32_t eip, eflags;
    uint32_t eax, ecx, edx, ebx;
    uint32_t esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldt;
    uint16_t trap, iomap;
} __attribute__((packed));

static struct tss_entry tss;

void tss_set_kernel_stack(uint32_t stack) {
    tss.esp0 = stack;
}

void tss_init() {
    tss.ss0 = 0x10; // kernel data
    tss.esp0 = 0;
    uint32_t base  = (uint32_t)&tss;
    uint32_t limit = sizeof(tss);
    gdt_set_tss(5, base, limit);
    asm volatile("ltr %%ax" :: "a"(0x28)); // TSS selector
}
