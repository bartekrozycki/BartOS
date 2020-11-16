#include "memory_manager.h"
#include "palloc.h"
#include "bitmap.h"
#include "system.h"

#define FRAME_N(address) (address >> 12)

// /**
//  * @return address of free page (0x1000)
//  */
// void *kalloc()
// {
//     u32 x = ms_pop();
//     bitmap_set(FRAME_N(x), ALLOCATED);
//     return (void *) x;
// }
// /**
//  * @param x address of page to free
//  */
// void *kfree(void *x)
// {
//     if ((u32)x & 0xFFF) kernel_panic(SERIAL, "KFREE addres not aligned to 0x1000");

//     ms_push((u32 *)x);
//     bitmap_set(FRAME_N((u32)x), FREE);
// }

