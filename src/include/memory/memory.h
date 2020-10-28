#pragma once
#include "ints.h"
#include "multiboot.h"

#define KERNEL_START 0x100000


void init_memory(MultibootInfo* mbi);
void memory_map(MultibootInfo *mbi);
u8* mmu_malloc(u32 size);
