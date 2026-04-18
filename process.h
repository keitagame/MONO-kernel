// process.h
#pragma once
#include <stdint.h>

typedef enum {
    PROC_UNUSED,
    PROC_READY,
    PROC_RUNNING,
    PROC_SLEEPING,
    PROC_ZOMBIE
} proc_state_t;

struct trapframe {
    uint32_t edi, esi, ebp, esp_dummy, ebx, edx, ecx, eax;
    uint32_t eip, cs, eflags, useresp, ss;
};

typedef struct process {
    proc_state_t state;
    uint32_t pid;
    uint8_t*  kstack;        // カーネルスタック
    struct trapframe* tf;    // 割り込み時のレジスタ保存領域
} process_t;

#define MAX_PROC 16
extern process_t proc_table[MAX_PROC];
extern process_t* current;
