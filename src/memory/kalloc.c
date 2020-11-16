#include "kalloc.h"

/**
 * @return address of free page (0x1000)
 */
void *kalloc()
{
    u32 x = (u32) ms_pop();
    bitmap_set(PAGE(x), ALLOCATED);
    return (void *) x;
}
/**
 * @param x address of page to free
 */
void kfree(void *x)
{
    if ((u32)x & 0xFFF) kernel_panic(SERIAL, "KFREE addres not aligned to 0x1000");

    ms_push((u32 *)x);
    bitmap_set(PAGE((u32)x), FREE);
}

