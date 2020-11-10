#include "mem.h"
#include "ints.h"
#include "print.h"
#include "multiboot.h"
#include "paging.h"
#include "system.h"
#include "lib.h"

// = > = > = > = > = > = > = > = > end of kernel

static u32 mem_in_kib;
static u32 highest_address;

void init_memory(MultibootInfo* mbi)
{
    if (mbi->flags & MULTIBOOT_FLAG_MEM)
        print(SERIAL, "[MMU] Multiboot found free %d %d => %d KiB memory\n", mbi->mem_lower, mbi->mem_upper, mem_in_kib = mbi->mem_lower + mbi->mem_upper);
    else 
        kernel_panic(TERMINAL, "[MMU] Multboot `mem_lower` & `mem_upper` failed");

    if (mbi->flags & MULTIBOOT_FLAG_MMAP)
        memory_map(mbi);
    else 
        kernel_panic(TERMINAL, "[MMU] Multiboot `mmap` failed");
}

void memory_map(MultibootInfo *mbi)
{
    ASSERT(MultibootMemoryMap, 24);

    MultibootMemoryMap 	*mmap = (MultibootMemoryMap *) mbi->mmap_address;
    u32 temp;
    u8 *bitmap;

    // fine highest address
    while ((u32) mmap < (mbi->mmap_address + mbi->mmap_length))
    {
        if (mmap->type == MULTIBOOT_MMAP_FREE_MEMORY)
        {
            highest_address = mmap->baselow + mmap->lenlow;
        }
        mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size));
    }

    mmap = (MultibootMemoryMap *) mbi->mmap_address;
    bitmap = createMemMap(highest_address >> 12); // right shift 12 ==> (x / 0x1000) ==> x / page_size
    
    while ((u32) mmap < (mbi->mmap_address + mbi->mmap_length))
    {
        if (mmap->type == MULTIBOOT_MMAP_FREE_MEMORY)
        {
            for (u32 i = 0; i < mmap->lenlow; i += 0x1000)
            {
                temp = mmap->baselow + i;
                map_page((void *)temp, (void *)temp, 0x0);
                
                if (temp >= 0x100000 && temp <= getKernelBrk())
                    mm_status_set(temp >> 12, SYSTEM);
                else
                    mm_status_set(temp >> 12, FREE);
            }
        }
        if (mmap->type == MULTIBOOT_MMAP_RESERVED)
        {
            if (mmap->baselow < highest_address)
            {
                for (u32 i = 0; i < mmap->lenlow; i += 0x1000)
                {
                    temp = mmap->baselow + i;
                    map_page((void *)temp, (void *)temp, 0x0);
                    mm_status_set(temp >> 12, SYSTEM);
                }
            }
        }
        mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size));
    }
    
}