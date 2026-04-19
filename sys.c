// sys.c
#include "sys.h"
#include "serial.h"
#include "process.h"

ssize_t sys_write(int fd, const void *buf, size_t count) {
    (void)fd; // 今は 1=stdout だけ扱う想定でもOK
    const char *p = buf;
    for (size_t i = 0; i < count; i++) {
        serial_write_char(p[i]);
    }
    return (ssize_t)count;
}

void sys_exit(int status) {
    (void)status;
    // 今は「プロセスを止める」概念がないので、とりあえず無限ループ
    while (1) {
        __asm__ volatile ("hlt");
    }
}

int sys_getpid(void) {
    return current ? current->pid : 0;
}
