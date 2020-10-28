#include "memory.h"
#include "ints.h"
#include "print.h"
#include "multiboot.h"
#include "system.h"
#include "lib.h"

// = > = > = > = > = > = > = > = > end of kernel
extern u32 _kernel_end;
static u8* kernel_end = (u8 *)&_kernel_end;

static u32 mem_in_kb;


void init_memory(MultibootInfo* mbi)
{
    if (mbi->flags & MULTIBOOT_FLAG_MEM)
        print(SERIAL, "[MMU] Multiboot found %d KiB memory\n", mem_in_kb = mbi->mem_lower + mbi->mem_upper);
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
    
    while ((u32) mmap < (mbi->mmap_address + mbi->mmap_length))
    {
        if (mmap->type == MULTIBOOT_MMAP_FREE_MEMORY)
        {
            // if (mmap->baselow >= 0x100000) continue; // bochs bios showing bellow 1mb free space

            print(SERIAL, "TODO MAPING FOR %p\n", mmap->baselow);
        }
        mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size));
    }
}
void* kalloc_frame()
{

}