// process.c
#include "process.h"
#include "pmm.h"
#include <stddef.h>
void *memset(void *dst, int value, size_t size) {
    unsigned char *p = (unsigned char *)dst;
    while (size--) {
        *p++ = (unsigned char)value;
    }
    return dst;
}
process_t proc_table[MAX_PROC];
process_t* current = 0;
uint32_t next_pid = 1;
/*
process_t* process_create(void (*entry)(void))
{
    for (int i = 0; i < MAX_PROC; i++) {
        if (proc_table[i].state == PROC_UNUSED) {
            process_t* p = &proc_table[i];
            p->state = PROC_READY;
            p->pid = next_pid++;

            // カーネルスタック確保
            p->kstack = pmm_alloc_page(); // 4KB で十分
            uint32_t stack_top = (uint32_t)p->kstack + 4096;

            // トラップフレームをスタックに作る
            stack_top -= sizeof(struct trapframe);
            p->tf = (struct trapframe*)stack_top;

            // 初期レジスタ
            p->tf->eip = (uint32_t)entry;
            p->tf->cs = 0x08;
            p->tf->eflags = 0x202;

            return p;
        }
    }
    return 0;
}
*/
process_t* process_create(void (*entry)(void))
{
    for (int i = 0; i < MAX_PROC; i++) {
        if (proc_table[i].state == PROC_UNUSED) {
            process_t* p = &proc_table[i];
            p->state = PROC_READY;
            p->pid = next_pid++;

            p->kstack = pmm_alloc_page();
            uint32_t stack_top = (uint32_t)p->kstack + 4096;

            stack_top -= sizeof(struct trapframe);
            p->tf = (struct trapframe*)stack_top;

            // ★ まず全部 0 でクリア
            memset(p->tf, 0, sizeof(*p->tf));

            // ★ 初期レジスタ
            p->tf->eip    = (uint32_t)entry;
            p->tf->cs     = 0x08;
            p->tf->eflags = 0x202;
            p->tf->esp_dummy = stack_top + sizeof(struct trapframe);
            // ★ 必要ならここで「タスク開始時に使うスタック」を決める
            // 例: カーネルモードでそのまま動かすなら、
            // p->tf->useresp = stack_top + sizeof(struct trapframe); // など
            // （ただしこれは switch_to の実装次第）

            return p;
        }
    }
    return 0;
}