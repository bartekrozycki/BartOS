#include "mmu.h"
#include "ints.h"
#include "print.h"
#include "multiboot.h"
#include "system.h"
#include "lib.h"

// = > = > = > = > = > = > = > = > end of kernel
extern u32 _kernel_end;
static u8* kernel_end = (u8 *)&_kernel_end;

static u32 mem_in_kb;



void init_mmu(MultibootInfo* mbi)
{
    if (mbi->flags & MULTIBOOT_FLAG_MEM)
        print(SERIAL, "[MMU] Multiboot found %d KiB memory\n", mem_in_kb = mbi->mem_lower + mbi->mem_upper);
    else 
        kernel_panic(TERMINAL, "[MMU] Multboot `mem_lower` & `mem_upper` failed");

    if (mbi->flags & MULTIBOOT_FLAG_MMAP)
        mmu_map(mbi);
    else 
        kernel_panic(TERMINAL, "[MMU] Multiboot `mmap` failed");

}

void mmu_map(MultibootInfo *mbi)
{

    ASSERT(MultibootMemoryMap, 24);
    MultibootMemoryMap 	*mmap = (MultibootMemoryMap *) mbi->mmap_address;

    while ((u32) mmap < (mbi->mmap_address + mbi->mmap_length))
    {
        if (mmap->type == MULTIBOOT_MMAP_FREE_MEMORY)
        {
            for (u32 i = 0; i < mmap->lenlow; i += 0x1000)
            {
                if (mmap->baselow + i > 0xFFFFFFFF) 
                    break;
                // mmu_bitmap_set(page_id(mmap->baselow + i), FREE);
            }
        }
        else 
        {
            for (u32 i = 0; i < mmap->lenlow; i += 0x1000)
            {
                if (mmap->baselow + i > 0xFFFFFFFF) 
                    break;
                // mmu_bitmap_set(page_id(mmap->baselow + i), RESERVED);
            }
        }
        mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size));
    }

    // marking kernel as reserved
    // for (u32 i = 0; (i + KERNEL_START) < ((u32)kernel_end + KERNEL_START); i += 0x1000)
    //     mmu_bitmap_set(page_id(mmap->baselow + i), RESERVED);
}

u8* mmu_malloc(u32 size)
{
    u8 *address = kernel_end;

    kernel_end += size;

    print(SERIAL, "[MMU] Allocated space from %p to %p. Total size %u bytes.\n", address, kernel_end, size);
    return address;
}