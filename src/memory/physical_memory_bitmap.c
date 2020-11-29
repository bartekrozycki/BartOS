#include "physical_memory_bitmap.h"
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
 * @param page_number
 * @param page_status
 * \page_status FREE / ALLOCATED / SYSTEM / HOLE
 */
void bitmap_set(u32 page_number, PageStatus page_status)
{
    if (!bitmap_address) kPanic;
    
    u32 *ptr = (u32 *)bitmap_address;
    ptr += (page_number / 16);

    u32 new_val = ((u32)page_status) << (30 - ((page_number % 16) * 2));
    u32 mask = ((u32)0b11) << (30 - ((page_number % 16) * 2));

    *ptr = ((*ptr & ~mask) | new_val);
}