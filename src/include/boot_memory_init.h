#pragma once
#include "ints.h"
#include "multiboot.h"
#include "paging.h"
#include "mem_map.h"

#define FRAMES_COUNT(x) (x >> 12)
#define ALIGN_TO(x,mask) (((x) + (mask) - 1) & ~((mask) - 1))

void boot_init_mem(u32 mboot_magic, MultibootInfo* mbi, u32 *kernel_start, u32 *kernel_end);
void* ke_alloc(u32* end, u32 size);
void map_at(PageDirectory * page_directory, void * physaddr, void * virtualaddr);

static inline void clearSpace(void* address, u32 len)
{
    for (u32 i = 0; i < len ; i++)
        ((u8*)address)[i] = 0;
}

static inline void enablePaging(PageDirectory* pd_addr)
{
    __asm__ ("movl %0, %%cr3": : "a"(pd_addr));
    __asm__ ("movl %cr0, %eax ");

    __asm__ ("movl $0x80000000, %ebx");
    __asm__ ("orl %ebx, %eax");
    __asm__ ("movl %eax, %cr0");

    __asm__ ("addl $KERNEL_HIGH_VMA, %esp");
    
}
static inline u32 get_highest_adrress(MultibootInfo* mbi)
{
    u32 ret;
    for (MultibootMemoryMap *mmap = (MultibootMemoryMap *) mbi->mmap_address; (u32) mmap < (mbi->mmap_address + mbi->mmap_length); mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size)))
        if (mmap->type == MULTIBOOT_MMAP_FREE_MEMORY)
            ret = mmap->baselow + mmap->lenlow;
    return ret;
}
