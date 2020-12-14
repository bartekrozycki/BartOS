#include "kernel_kalloc.h"
#include "physical_memory_stack.h"
#include "physical_memory_bitmap.h"
#include "kernel_panic.h"
#include "multiboot.h"
#include "acpi.h"

void init_kalloc(MultibootInfo *mbi, u32 kernel_start, u32 kernel_end) {
    bitmap_set((u32) mbi >> 12, SYSTEM);
    for (MultibootMemoryMap *mmap = (MultibootMemoryMap *) mbi->mmap_address;
         (u32) mmap < (mbi->mmap_address + mbi->mmap_length);
         mmap = (MultibootMemoryMap *) ((u32) mmap + mmap->size + sizeof(mmap->size))) {
        if (mmap->basehigh) continue;

        if (mmap->type == MULTIBOOT_MMAP_FREE_MEMORY) {
            for (u32 i = 0x0; i < mmap->lenlow; i += 0x1000) {
                if (((mmap->baselow + i) < kernel_start) || ((mmap->baselow + i) >= kernel_end))
                    ms_push((u32 *) (mmap->baselow + i));
                else
                    bitmap_set((u32) ((mmap->baselow + i) >> 12), SYSTEM);
            }
        } else if (mmap->type == MULTIBOOT_MMAP_RESERVED && mmap->baselow < KERNEL_BOOT_VMA)
            for (u32 i = 0x0; i < mmap->lenlow; i += 0x1000)
                bitmap_set((u32) ((mmap->baselow + i) >> 12), SYSTEM);
        else if (mmap->type == MULTIBOOT_MMAP_ACPI)
            acpi_address = (u32 *) mmap->baselow;

    }
}

/**
 * @param status PageStatus
 * @return address of free page (0x1000)
 */
void *k_alloc(enum PageStatus status)
{
    u32 x = (u32) ms_pop();
    bitmap_set(PAGE(x), status);
    return (void *) x;
}

/**
 * @param x address of page to free
 */
void kfree(void *x) {
    if ((u32) x & 0xFFF)
        kPanic;

    ms_push((u32 *) x);
    bitmap_set(PAGE((u32) x), FREE);
}
