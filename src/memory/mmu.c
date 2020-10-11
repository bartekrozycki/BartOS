#include "mmu.h"
#include "ints.h"
#include "print.h"
#include "multiboot.h"
#include "system.h"
#include "lib.h"

#define MMU_PAGES(KiB) ((KiB)/4)
#define MMU_BITMAP_SPACE_IN_BYTES(KiB) ((MMU_PAGES(KiB)/8)+1)

// = > = > = > = > = > = > = > = > end of kernel
extern u32 _kernel_end;
static u8* kernel_end = (u8 *)&_kernel_end;

static u32 mem_in_kb;
static u32 mem_pages_count;

u32* memory_bitmap;


void init_mmu(MultibootInfo* mbi)
{
    if (mbi->flags & MULTIBOOT_FLAG_MEM)
    {
        print(SERIAL, "[MMU] Multiboot found %d KiB memory\n", mem_in_kb = mbi->mem_lower + mbi->mem_upper);
    }
    else 
    {
        kernel_panic(TERMINAL, "[MMU] Multboot `mem_lower` & `mem_upper` failed");
    }   

    mem_pages_count = MMU_PAGES(mem_in_kb);
        
    //alloc required space after kenel_end
    memory_bitmap = (u32 *) mmu_malloc(MMU_BITMAP_SPACE_IN_BYTES(mem_in_kb));

    //set everythink to reserved - not avaible 
    memset(memory_bitmap, BYTE_RESERVED, MMU_BITMAP_SPACE_IN_BYTES(mem_in_kb));

    if (mbi->flags & MULTIBOOT_FLAG_MMAP)
    {
        mmu_map(mbi);
    }
    else 
    {
        kernel_panic(TERMINAL, "[MMU] Multiboot `mmap` failed");
    }

}

void mmu_map(MultibootInfo *mbi)
{

    ASSERT(MultibootMemoryMap, 24);

    MultibootMemoryMap 	*mmap = (MultibootMemoryMap *) mbi->mmap_address;

    /*
        default all pages is set to reserved [ bitmap ]
    */

    while ((u32) mmap < (mbi->mmap_address + mbi->mmap_length))
    {
        if (mmap->type == MULTIBOOT_MMAP_FREE_MEMORY)
        {
            for (u32 i = 0; i < mmap->lenlow; i += 0x1000)
            {
                if (mmap->baselow + i > 0xFFFFFFFF) 
                    break;
                mmu_bitmap_set(page_id(mmap->baselow + i), FREE);
            }
        }
        else 
        {
            for (u32 i = 0; i < mmap->lenlow; i += 0x1000)
            {
                if (mmap->baselow + i > 0xFFFFFFFF) 
                    break;
                mmu_bitmap_set(page_id(mmap->baselow + i), RESERVED);
            }
        }
        mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size));
    }

    // marking kernel as reserved
    for (u32 i = 0; (i + KERNEL_START) < ((u32)kernel_end + KERNEL_START); i += 0x1000)
        mmu_bitmap_set(page_id(mmap->baselow + i), RESERVED);
}

u8* mmu_malloc(u32 size)
{
    u8 *address = kernel_end;

    kernel_end += size;

    print(SERIAL, "[MMU] Allocated space from %p to %p. Total size %u bytes.\n", address, kernel_end, size);
    return address;
}

u32 mmu_get_pages_count()
{
    return mem_pages_count;
}