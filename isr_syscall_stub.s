# isr_syscall_stub.S
.global isr_syscall_stub
isr_syscall_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es

    mov 44(%esp), %eax   # syscall number
    mov 48(%esp), %ebx   # arg1
    mov 52(%esp), %ecx   # arg2
    mov 56(%esp), %edx   # arg3

    push %edx
    push %ecx
    push %ebx
    push %eax
    call syscall_dispatch
    add $16, %esp

    mov %eax, 44(%esp)

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    iret
