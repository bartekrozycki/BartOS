#include "heap.h"
#include "mem_map.h"
#include "kalloc.h"
#include "print.h"
#include "kernel_panic.h"

static u32 *heap_address;
/**
 * @param kernel_end end of kernel adress
 * @param directory Page directory address
 */
void init_heap()
{
    u32* temp;
    for (u32 i = 0; i < HEAP_SIZE; i += 0x1000)
    {
        temp = kalloc();
        map((u32)temp, HEAP_SPACE + i);
    }
    heap_address = HEAP_SPACE;
    print(SERIAL, "Heap initialized at %p total size %dMiB\n", HEAP_SPACE, HEAP_SIZE >> 20);
}
