#pragma once
#include "ints.h"
#include "multiboot.h"
#include "paging.h"
#include "mStack.h"
#include "mem_map.h"

#define FRAMES_COUNT(x) (x >> 12)
#define ALIGN_TO(x,mask) (((x) + (mask) - 1) & ~((mask) - 1))


void boot_init_mem(u32 mboot_magic, MultibootInfo* mbi);
void init_memory(MultibootInfo *mbi, u32 *kernel_start, u32 *kernel_end);


u32 get_highest_adrress(MultibootInfo* mbi, u32 *ret);
u32 ke_alloc(u32* end, u32 size);


void initial_kernel_paging(u32 * ke, PageDirectory *pd, MultibootInfo *mbi);


void map(u32 *ke, PageDirectory * pd, u32 physaddr, u32 virtualaddr);
void enablePaging(PageDirectory* pd_addr);
