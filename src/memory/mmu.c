#include "mmu.h"
#include "ints.h"
#include "print.h"
#include "multiboot.h"
#include "system.h"
#include "lib.h"

#define MMU_PAGES(KiB) ((KiB)/4)
#define MMU_BITMAP_SPACE_IN_BITS(KiB) (MMU_PAGES(KiB)/4)

extern u32 _kernel_end;

static u8* kernel_end = (u8 *)&_kernel_end;
static u32 mem_in_kb;

static u32* bitmap;

void init_mmu(MultibootInfo* mbi)
{
    if (mbi->flags & MULTIBOOT_FLAG_MEM)
        print(SERIAL, "[MMU] Initialized %d KiB\n", mem_in_kb = mbi->mem_lower + mbi->mem_upper);
    else kernel_panic(TERMINAL, "[MMU] Multboot `mem_lower` & `mem_upper` failed");

    //alloc required space after kenel_end
    bitmap = (u32 *) mmu_malloc(MMU_BITMAP_SPACE_IN_BITS(mem_in_kb) + 1);
    //set everythink to reserved
    memset(bitmap, RESERVED_BYTE, MMU_BITMAP_SPACE_IN_BITS(mem_in_kb) + 1);

    if (mbi->flags & MULTIBOOT_FLAG_MMAP)
        mmu_map(mbi);
    else kernel_panic(TERMINAL, "[MMU] Multiboot `mmap` failed");

}

void mmu_map(MultibootInfo *mbi)
{
    MultibootMemoryMap 	*mmap = (MultibootMemoryMap *) mbi->mmap_address;

    while ((u32) mmap < (mbi->mmap_address + mbi->mmap_length))
    {
        print(TERMINAL, "Base %x%x    Len %x%x   Type %x\n", mmap->base_high, mmap->base_low, mmap->len_high, mmap->len_low, mmap->type);
        // switch (mmap->type)
        // {
        // 	case MULTIBOOT_MMAP_FREE_MEMORY:
        // 		print(SERIAL, "FREE_MEM: From %p  To %p  %p\n", mmap->base_address, mmap->length, mmap->size);
        // 		break;
        // 	case MULTIBOOT_MMAP_RESERVED:
        // 		print(SERIAL, "RESERVED MEM:  %p  To %p  %p\n", mmap->base_address, mmap->length, mmap->size);
        // 		break;
        // }
        mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size));
    }
}

u8* mmu_malloc(u32 size)
{
    u8 *address = kernel_end;

    kernel_end += size;

    print(SERIAL, "[MMU] Allocated bitmap from %p to %p. Total size %u bytes.\n", address, kernel_end, size);
    return address;
}