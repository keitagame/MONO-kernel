// pmm.h
#pragma once
#include <stdint.h>
#include <stddef.h>
#include "multiboot.h"
void pmm_init(struct multiboot_info* mb);
void* pmm_alloc_page(void);
void  pmm_free_page(void* p);
