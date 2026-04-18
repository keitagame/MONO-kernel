.global irq0_handler
irq0_handler:
    pusha
    call timer_handler   # C のスケジューラ呼び出し
    popa

    # PIC へ EOI
    mov $0x20, %al
    out %al, $0x20

    iret

.global gdt_flush
gdt_flush:
    mov 4(%esp), %eax
    lgdt (%eax)

    mov $0x10, %ax      
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    ljmp $0x08, $1f     
1:
    ret
