// tss.h
#ifndef TSS_H
#define TSS_H

#include <stdint.h>

void tss_init(void);
void tss_set_kernel_stack(uint32_t stack);

#endif
