#include "mem.h"
#include "print.h"
#include "lib.h"

static u8 *kernel_brk =  (u8 *)&_kernel_end;
static u8 *kernel_mem_brk = (u8 *)0xE0400000; 

u32 getKernelBrk(void) { return (u32) PhysicalAddress(kernel_brk); }

// size * 0x1000
void *_kalloc_frame(unsigned int size)
{
    if ((kernel_brk + (size * 0x1000)) >= kernel_mem_brk) return (void *)0;

    if (getBitmap())
        for (unsigned int x = 0; x < (size * 0x1000); x += 0x1000)
            mm_status_set( (u32)(PhysicalAddress(kernel_brk) + x) >> 12, SYSTEM);
    
        
    memset(kernel_brk, 0, 0x1000 * size);

    kernel_brk += (size * 0x1000);
   
    return PhysicalAddress(kernel_brk - (size * 0x1000));
}