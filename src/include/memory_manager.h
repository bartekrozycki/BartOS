#pragma once
#include "ints.h"
#include "multiboot.h"
#include "paging.h"
#include "mStack.h"

extern u32 _kernel_start;
extern u32 _kernel_end;


void memory_manager(u32 mboot_magic, MultibootInfo* mbi);

u32 get_highest_adrress(MultibootInfo* mbi, u32 *ret);
u32 ke_alloc(u32* end, u32 size);

void initial_kernel_paging(u32 * ke, PageDirectory *pd);
void map(u32 *ke, PageDirectory * pd, u32 physaddr, u32 virtualaddr);

void enablePaging(PageDirectory* pd_addr);

void init_physical(MultibootInfo* mbi);

