// sys.h
#pragma once
#include <stdint.h>
#include <stddef.h>


typedef long ssize_t;

ssize_t sys_write(int fd, const void *buf, size_t count);
void    sys_exit(int status);
int     sys_getpid(void);
