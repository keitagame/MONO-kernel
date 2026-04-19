


.set MULTIBOOT_MAGIC,  0x1BADB002
.set MULTIBOOT_FLAGS,  0x0
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

.section .multiboot
    .align 4
    .long MULTIBOOT_MAGIC
    .long MULTIBOOT_FLAGS
    .long MULTIBOOT_CHECKSUM

.section .bss
    .align 16
stack_bottom:
    .skip 16384        # 16KB stack
stack_top:

.section .text
    .global _start
    .extern kernel_main

_start:
    cli
    mov $stack_top, %esp
    push %eax          # multiboot magic
    push %ebx          # multiboot info struct pointer
    call kernel_main
.hang:
    cli
    hlt
    jmp .hang
