#pragma once
#include "ints.h"
#include "multiboot.h"
#include "paging.h"

/*
 * kernel code and static data
 */
extern u32 _kernel_start;
extern u32 _kernel_end;

/*
 * Early Kernel Allocator
 * 
 * can cover alloc memory only up to 0xE0400000
 * kernel start in 0xE0100000
 * only ~3MiB can be allocated via this function
 * 
 * getKernelBrk(void) <=> returning first free page 
*/

#define PhysicalAddress(x) ( ((unsigned int)x) - 0xE0000000 )

void *_kalloc_frame(unsigned int size);

u32 getKernelBrk(void);


typedef enum PageStatus {
    FREE = 0,
    ALLOCATED,
    SYSTEM,
    HOLE
} PageStatus;

u8 *createMemMap(unsigned int size); // size in KiB
u8 *getBitmap();

u32 mm_bitmap_getSize();
u32 mm_bitmap_getPagesCount();


void mm_status_set(unsigned int n, PageStatus status);
u8 mm_status_get(unsigned int n);


void init_memory(MultibootInfo* mbi);
void memory_map(MultibootInfo *mbi);
u8* mmu_malloc(u32 size);
