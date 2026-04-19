# switch.S
.global switch_to
# void switch_to(struct trapframe* old, struct trapframe* new);
switch_to:
    # new のポインタを esi に保持（絶対に上書きしない）
    mov 8(%esp), %esi        # esi = new

    # ---- new->汎用レジスタをロード ----
    mov 0(%esi), %edi        # edi
    mov 4(%esi), %edx        # esi フィールドは edx に入れる
    mov 8(%esi), %ebp        # ebp
    mov 16(%esi), %ebx       # ebx
    mov 20(%esi), %edx       # edx
    mov 24(%esi), %ecx       # ecx
    mov 28(%esi), %eax       # eax

    # ---- スタックポインタ切り替え ----
    mov 12(%esi), %esp       # esp = esp_dummy

    # ---- EIP にジャンプ ----
    jmp *32(%esi)            # eip
