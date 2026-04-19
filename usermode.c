// usermode.c
#include <stdint.h>
void enter_user_mode(uint32_t entry, uint32_t user_stack) {
    asm volatile(
        "cli\n"
        "mov $0x23, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"

        "pushl $0x23\n"
        "pushl %0\n"
        "pushfl\n"
        "popl %%eax\n"
        "or $0x200, %%eax\n"
        "pushl %%eax\n"
        "pushl $0x1B\n"
        "pushl %1\n"
        "iret\n"
        :
        : "r"(user_stack), "r"(entry)
        : "eax"
    );
}
