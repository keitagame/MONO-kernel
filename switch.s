# switch.S
.global switch_to
switch_to:
    # 引数: old (eax), new (edx)

    # old->tf にレジスタを保存
    mov 4(%esp), %eax   # old
    mov 8(%esp), %edx   # new

    # old が NULL の場合は保存しない
    test %eax, %eax
    jz 1f

    # old->tf->edi = edi など
    mov %edi, 0(%eax)
    mov %esi, 4(%eax)
    mov %ebp, 8(%eax)
    mov %ebx, 16(%eax)
    mov %edx, 20(%eax)
    mov %ecx, 24(%eax)
    mov %eax, 28(%eax)
1:

    # new->tf をロード
    mov 8(%esp), %edx
    mov 0(%edx), %edi
    mov 4(%edx), %esi
    mov 8(%edx), %ebp
    mov 16(%edx), %ebx
    mov 20(%edx), %edx
    mov 24(%edx), %ecx
    mov 28(%edx), %eax

    # EIP, CS, EFLAGS は iret で戻す
    add $32, %esp
    iret
