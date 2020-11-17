#include "heap.h"
#include "boot_memory_init.h"
#include "mem_map.h"
#include "print.h"

static u32 *heap_address;
/**
 * @param kernel_end end of kernel adress
 * @param directory Page directory address
 */
void init_heap(u32 * kernel_end, PageDirectory *directory)
{

}

//TODO MALLOCCCC