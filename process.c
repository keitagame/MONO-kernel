// process.c
#include "process.h"
#include "pmm.h"

process_t proc_table[MAX_PROC];
process_t* current = 0;
uint32_t next_pid = 1;

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
