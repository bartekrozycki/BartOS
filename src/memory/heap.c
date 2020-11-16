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
    u32 heap = ke_alloc(kernel_end, 0x100000 * HEAP_MIB_SPACE);

    for (u32 i = 0; i < 0x100000 * HEAP_MIB_SPACE; i+= 0x1000)
        kmap(directory, heap + i, HEAP_SPACE + i);
    
    heap_address = (u32 *) HEAP_SPACE;
    print(SERIAL, "[Kernel] Heap initalized at %p\n", heap_address);
}

//TODO MALLOCCCC