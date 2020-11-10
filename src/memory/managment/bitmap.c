#include "mem.h"
#include "print.h"
#include "lib.h"
#include "system.h"

/*
    bitmap can cover up to 64 pages -> 64 * 0x1000 ----> 4GiB
*/

// address of bitmap (after createMemMap)
static u8 *bitmap = 0;
// bytes need for all pages
static u32 bitmap_size = 0;
// how much pages are stored in memory bitmap
static u32 bitmap_pages_count = 0; 

u8 *getBitmap() { return bitmap; }
u32 mm_bitmap_getSize() { return bitmap_size; }
u32 mm_bitmap_getPagesCount() { return bitmap_pages_count; } 

u8 *createMemMap(unsigned int pages_count)
{
    bitmap_pages_count = pages_count;
    bitmap_size = (pages_count / 4); // bytes need for all pages

    bitmap = _kalloc_frame((bitmap_size / 0x1000) + 1); // 32mb need 0.5 page (int) would be 0 so + 1
    memset(bitmap,bitmap_size, FREE);

    return bitmap;
}

u8 mm_status_get(unsigned int n)
{
    u32* ptr = (u32 *) bitmap;
    ptr += (n / 16); // 

    return (PageStatus) (*ptr >> (30 - ( (n % 16) * 2)) & 0b11);
}

void mm_status_set(unsigned int n, PageStatus status)
{
    u32 *ptr =  (u32 *) bitmap;
    ptr += (n / 16);

    u32 newval = ((u32)status) << (30 - ( (n % 16) * 2));
    u32 mask = ((u32) 0b11) << (30 - ( (n % 16) * 2));

    *ptr = ( (*ptr & ~mask) | newval);
}
