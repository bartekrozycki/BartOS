#include "mmu.h"

extern u32 * memory_bitmap; // declared mmu.c

mmu_page_status mmu_bitmap_get(u32 n)
{
    u32 *ptr = memory_bitmap;

    ptr += (n / 32);

    return (mmu_page_status) (*ptr >> (31 - ( n % 32 ))) & ((u32) 0b1);
}
void mmu_bitmap_set(u32 n, mmu_page_status status)
{
    u32 *ptr = memory_bitmap;
    ptr += (n / 32);

    u32 newval = ((u32)status) << (31 - ( n % 32 ));
    u32 mask = ((u32) 0b1) << (31 - ( n % 32 ));

    *ptr = ( (*ptr & ~mask) | newval);
}
u32 page_id(u32 addr)
{
    return addr / 0x1000;
}