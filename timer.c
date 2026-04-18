#include <stdint.h>
#include "serial.h"

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// PIC を 0x20〜0x2F に再配置（IRQ0 を IDT の 32 番にする）
static void pic_remap(void)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20); // master PIC offset = 0x20
    outb(0xA1, 0x28); // slave PIC offset = 0x28

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // IRQ0 だけ許可（他はマスク）
    outb(0x21, 0xFE); // 1111 1110
    outb(0xA1, 0xFF);
}

void timer_init(uint32_t freq)
{
    pic_remap();

    uint32_t divisor = 1193180 / freq;

    // PIT コマンドレジスタ
    outb(0x43, 0x36); // channel 0, lobyte/hibyte, mode 3

    // divisor を下位・上位に分けて送る
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);

    serial_write_string("PIT initialized.\n");
}
