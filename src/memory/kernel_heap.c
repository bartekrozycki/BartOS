#include "kernel_heap_malloc.h"
#include "kernel_heap.h"
#include "mem_map.h"
#include "kernel_kalloc.h"
#include "print.h"
#include "kernel_panic.h"
#include "ints.h"

static heap_t *heap = NULL;

///**
// * @param kernel_end end of kernel address
// * @param directory Page directory address
// */
// void *kalloc(size_t pages, u32 virtual_address)
//{
//
//}
void init_heap() {
    u32 *temp;

    for (u32 i = 0; i < HEAP_SIZE; i += 0x1000) {
        temp = k_alloc(SYSTEM);
        map((u32) temp, HEAP_SPACE + i);
    }

    /**
     *      initializing heap structure
     */

    heap = (heap_t *) HEAP_SPACE;

    block_t *head = (block_t *) (HEAP_SPACE + sizeof(heap_t)); // graniczny lewy
    block_t *tail = (block_t *) (HEAP_SPACE + HEAP_SIZE - sizeof(block_t)); // graniczny prawy
    block_t *free = (block_t *) ((u8 *) head + sizeof(block_t)); // pierwszy wolny blok

    *heap = (heap_t) {
            .heap_start = (void *) HEAP_SPACE,
            .head = head,
            .tail = tail,
    };
    *head = (block_t) {
            .p_prev = NULL,
            .p_next = free,
            .free = BLOCK_ALLOCATED,
            .size = 0,
    };
    *free = (block_t) {
            .p_prev = head,
            .p_next = tail,
            .free = BLOCK_FREE,
            .size = (u8 *) tail - (u8 *) head - sizeof(block_t),
    };
    *tail = (block_t) {
            .p_prev = free,
            .p_next = NULL,
            .free = BLOCK_ALLOCATED,
            .size = 0,
    };
#ifdef DEBUG
    print(SERIAL, "warning Heap Address: %p\n"
                  "warning Heap size total %p\n"
                  "warning Heap size available %p\n\n", heap->heap_start, HEAP_SIZE, free->size);
#endif

    print(SERIAL, "info Heap initialized\n\n");
}

heap_t* getHeap()
{
    if (!heap) kPanic;
    return heap;
}

void heap_block_join(block_t *p_ref) {
    p_ref->free = BLOCK_FREE;
    p_ref->size = p_ref->size + sizeof(block_t) + p_ref->p_next->size;

    block_t *p_last = p_ref->p_next->p_next;

    p_ref->p_next = p_last;
    p_last->p_prev = p_ref;
}