#include "bitmap.h"
#include "memory_manager.h"

extern MManager m;

/**
 * @param n page number
 * @return Page status
 */
PageStatus bitmap_get(u32 n)
{
    u32* ptr = (u32 *) m.Bitmap.address;
    ptr += (n / 16); // 

    return (PageStatus) (*ptr >> (30 - ( (n % 16) * 2)) & 0b11);
}

/**
 * @param n page number
 * @param status ennumerable page status
 */
void bitmap_set(u32 n, PageStatus status)
{
    u32 *ptr =  (u32 *) m.Bitmap.address;
    ptr += (n / 16);

    u32 newval = ((u32)status) << (30 - ( (n % 16) * 2));
    u32 mask = ((u32) 0b11) << (30 - ( (n % 16) * 2));

    *ptr = ( (*ptr & ~mask) | newval);
}