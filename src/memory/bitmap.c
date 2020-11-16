#include "bitmap.h"

static u32 * bitmap;

/**
 * @param address Address of bitmap
 */
void init_bitmap(u32* address)
{
    bitmap = address;
}

/**
 * @param n page number
 * @return Page status
 */
PageStatus bitmap_get(u32 n)
{
    u32* ptr = (u32 *) bitmap;
    ptr += (n / 16); // 

    return (PageStatus) (*ptr >> (30 - ( (n % 16) * 2)) & 0b11);
}

/**
 * @param n page number
 * @param status ennumerable page status
 */
void bitmap_set(u32 n, PageStatus status)
{
    u32 *ptr =  (u32 *) bitmap;
    ptr += (n / 16);

    u32 newval = ((u32)status) << (30 - ( (n % 16) * 2));
    u32 mask = ((u32) 0b11) << (30 - ( (n % 16) * 2));

    *ptr = ( (*ptr & ~mask) | newval);
}