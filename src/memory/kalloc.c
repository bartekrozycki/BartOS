#include "kalloc.h"
#include "mStack.h"
#include "bitmap.h"
#include "kernel_panic.h"
#include "multiboot.h"

void init_kalloc(MultibootInfo *mbi, u32 kernel_start, u32 kernel_end)
{
    bitmap_set((u32)mbi >> 12, SYSTEM);
    BOCHS_BREAK;
    for (MultibootMemoryMap *mmap = (MultibootMemoryMap *)mbi->mmap_address;
         (u32)mmap < (mbi->mmap_address + mbi->mmap_length);
         mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size)))
    {
        if (mmap->type == MULTIBOOT_MMAP_FREE_MEMORY)
        {
            for (u32 i = 0x0; i < mmap->lenlow; i += 0x1000)
            {
                if (((mmap->baselow + i) < kernel_start) || ((mmap->baselow + i) >= kernel_end))
                {
                    ms_push((u32 *)(mmap->baselow + i));
                }
                else
                {
                    BOCHS_BREAK;
                    bitmap_set((u32)((mmap->baselow + i) >> 12), SYSTEM);
                }
            }
        }
        else if (mmap->type == MULTIBOOT_MMAP_RESERVED)
        {
            for (u32 i = 0x0; i < mmap->lenlow; i += 0x1000)
            {
                BOCHS_BREAK;
                bitmap_set((u32)((mmap->baselow + i) >> 12), SYSTEM);
            }
        }
    }
}
/**
 * @return address of free page (0x1000)
 */
void *kalloc()
{
    u32 x = (u32)ms_pop();
    bitmap_set(PAGE(x), SYSTEM);
    return (void *)x;
}
/**
 * @param x address of page to free
 */
void kfree(void *x)
{
    if ((u32)x & 0xFFF)
        kPanic;

    ms_push((u32 *)x);
    bitmap_set(PAGE((u32)x), FREE);
}
