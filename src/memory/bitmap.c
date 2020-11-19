#include "bitmap.h"
#include "kernel_panic.h"

static u32 *bitmap_address;

void setBitmapAddress(u32 *address)
{
    bitmap_address = address;
}
/**
 * @param n page number
 * @return Page status
 */
PageStatus bitmap_get(u32 n)
{
    u32 *ptr = (u32 *)bitmap_address;
    ptr += (n / 16); //

    return (PageStatus)(*ptr >> (30 - ((n % 16) * 2)) & 0b11);
}

/**
 * @param n page number
 * @param status ennumerable page status
 */
void bitmap_set(u32 n, PageStatus status)
{
    if (!bitmap_address) kPanic;
    
    u32 *ptr = (u32 *)bitmap_address;
    ptr += (n / 16);

    u32 newval = ((u32)status) << (30 - ((n % 16) * 2));
    u32 mask = ((u32)0b11) << (30 - ((n % 16) * 2));

    *ptr = ((*ptr & ~mask) | newval);
}