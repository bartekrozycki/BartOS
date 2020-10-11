#pragma once
#include "ints.h"
#include "multiboot.h"

#define BYTE_RESERVED 0xFF
#define KERNEL_START 0x100000

typedef enum mmu_page_status {
    FREE        = 0x0,
    RESERVED    = 0x1
} mmu_page_status; 

void init_mmu(MultibootInfo* mbi);
void mmu_map(MultibootInfo *mbi);
u8* mmu_malloc(u32 size);

u32 page_id(u32 addr);
u32 mmu_get_pages_count();


mmu_page_status mmu_bitmap_get(u32 n);
void mmu_bitmap_set(u32 n, mmu_page_status status);