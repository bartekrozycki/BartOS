#pragma once
#include "ints.h"
#include "multiboot.h"

#define RESERVED_BYTE (RESERVED << 6 | RESERVED << 4 | RESERVED << 2 | RESERVED)

typedef enum mmu_page_status {
    FREE            = 0x0, 
    NOT_FREE        = 0x1, 
    RESERVED        = 0x2, 
    KERNEL          = 0x3 // 0b11 
} mmu_page_status; 

void init_mmu(MultibootInfo* mbi);
void mmu_map(MultibootInfo *mbi);
u8* mmu_malloc(u32 size);