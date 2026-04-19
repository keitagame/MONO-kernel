// gdt.c
#include "gdt.h"

static struct gdt_entry gdt[6];   // ★ 3 → 5 に増やす
static struct gdt_ptr   gdtp;

static void gdt_set_entry(int idx, uint32_t base, uint32_t limit,
                          uint8_t access, uint8_t gran)
{
    gdt[idx].limit_low = limit & 0xFFFF;
    gdt[idx].base_low  = base & 0xFFFF;
    gdt[idx].base_mid  = (base >> 16) & 0xFF;
    gdt[idx].access    = access;
    gdt[idx].gran      = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[idx].base_high = (base >> 24) & 0xFF;
}
void gdt_set_tss(int num, uint32_t base, uint32_t limit)
{
    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].base_low  = base & 0xFFFF;
    gdt[num].base_mid  = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].access = 0x89;  // present, ring0, type=0x9 (32-bit available TSS)

    gdt[num].gran = (limit >> 16) & 0x0F;
    gdt[num].gran |= 0x00;   // TSS は granularity=byte 単位
}
void gdt_init(void)
{
    gdtp.limit = sizeof(gdt) - 1;
    gdtp.base  = (uint32_t)&gdt;

    // null
    gdt_set_entry(0, 0, 0, 0, 0);
    // kernel code
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xCF);
    // kernel data
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xCF);
    // user code (ring3)
    gdt_set_entry(3, 0, 0xFFFFF, 0xFA, 0xCF);
    // user data (ring3)
    gdt_set_entry(4, 0, 0xFFFFF, 0xF2, 0xCF);

    gdt_flush((uint32_t)&gdtp);
}
