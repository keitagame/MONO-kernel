# switch.S
.global switch_to
# void switch_to(struct trapframe* old, struct trapframe* new);
switch_to:
    # 引数:
    #   [esp+4] = old
    #   [esp+8] = new

    # 今は old は使わない（常に 0 を渡している前提）
    # new のポインタを esi に保持しておく
    mov 8(%esp), %esi        # esi = new (struct trapframe*)

    # ---- new->汎用レジスタをロード ----
    # struct trapframe {
    #   0:  edi
    #   4:  esi
    #   8:  ebp
    #   12: esp_dummy
    #   16: ebx
    #   20: edx
    #   24: ecx
    #   28: eax
    #   32: eip
    #   ...
    # }

    mov 0(%esi), %edi        # edi
    mov 4(%esi), %edx        # esi フィールドはとりあえず edx に入れる（任意）
    mov 8(%esi), %ebp        # ebp
    mov 16(%esi), %ebx       # ebx
    mov 20(%esi), %edx       # edx
    mov 24(%esi), %ecx       # ecx
    mov 28(%esi), %eax       # eax

    # ---- スタックポインタをタスク用に切り替え ----
    mov 12(%esi), %esp       # esp = esp_dummy

    # ---- EIP にジャンプ（CS/eflags はカーネルのままでOK）----
    jmp *32(%esi)            # jmp new->eip
