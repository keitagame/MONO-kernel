// syscall.c
#include <stdint.h>
#include "serial.h"

enum {
    SYS_write = 1,
    SYS_exit  = 2,
};
/*
int sys_write(int fd, const char *buf, uint32_t len) {
    (void)fd;
    for (uint32_t i = 0; i < len; i++)
        serial_write_char(buf[i]);
    return len;
}

void sys_exit(int code) {
    serial_write_string("exit\n");
    while (1) asm volatile("hlt");
}
*/
uint32_t syscall_dispatch(uint32_t num, uint32_t a1, uint32_t a2, uint32_t a3) {
    switch (num) {
        case SYS_write: return sys_write(a1, (const char*)a2, a3);
        case SYS_exit:  sys_exit(a1); return 0;
    }
    return -1;
}
