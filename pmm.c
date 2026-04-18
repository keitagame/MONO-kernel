// pmm.c
#include "pmm.h"


#define PAGE_SIZE 4096
#define MAX_MEMORY_BYTES (128 * 1024 * 1024)
#define MAX_PAGES (MAX_MEMORY_BYTES / PAGE_SIZE)

static uint8_t bitmap[MAX_PAGES / 8];

static inline void bitmap_set(size_t idx)   { bitmap[idx / 8] |=  (1 << (idx % 8)); }
static inline void bitmap_clear(size_t idx) { bitmap[idx / 8] &= ~(1 << (idx % 8)); }
static inline int  bitmap_test(size_t idx)  { return bitmap[idx / 8] &   (1 << (idx % 8)); }

static void mark_region(uint64_t addr, uint64_t len, int usable)
{
    uint64_t start = addr / PAGE_SIZE;
    uint64_t end   = (addr + len) / PAGE_SIZE;

    if (end > MAX_PAGES) end = MAX_PAGES;

    for (uint64_t i = start; i < end; i++) {
        if (usable)
            bitmap_clear(i);
        else
            bitmap_set(i);
    }
}

void pmm_init(struct multiboot_info* mb)
{
    // まず全部使用中にしておく
    for (size_t i = 0; i < sizeof(bitmap); i++)
        bitmap[i] = 0xFF;

    if (!(mb->flags & MULTIBOOT_FLAG_MMAP))
        return;

    uint32_t mmap_end = mb->mmap_addr + mb->mmap_length;
    for (uint32_t p = mb->mmap_addr; p < mmap_end; ) {
        struct multiboot_mmap_entry* e = (struct multiboot_mmap_entry*)p;
        if (e->type == 1) { // usable
            mark_region(e->addr, e->len, 1);
        }
        p += e->size + sizeof(e->size);
    }

    // カーネル自身の領域はあとで reserved にする（TODO）
}

void* pmm_alloc_page(void)
{
    for (size_t i = 0; i < MAX_PAGES; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            return (void*)(i * PAGE_SIZE);
        }
    }
    return 0;
}

void pmm_free_page(void* p)
{
    uint32_t addr = (uint32_t)p;
    size_t idx = addr / PAGE_SIZE;
    if (idx < MAX_PAGES)
        bitmap_clear(idx);
}
