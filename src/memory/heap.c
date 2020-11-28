#include "heap.h"
#include "mem_map.h"
#include "kalloc.h"
#include "print.h"
#include "kernel_panic.h"
#include "ints.h"

static heap_t *heap;

/**
 * @param kernel_end end of kernel address
 * @param directory Page directory address
 */
void init_heap() {
    u32 *temp;

    for (u32 i = 0; i < HEAP_SIZE; i += 0x1000) {
        temp = kalloc();
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
            .current_id = HEAP_FIRST_UID,
            .unused = 0xBEEF
    };
    *head = (block_t) {
            .prev = NULL,
            .next = free,
            .free = 0,
            .size = 0,
            .id = HEAP_HEAD_UID
    };
    *free = (block_t) {
            .prev = head,
            .next = tail,
            .free = 1,
            .size = (u8 *) tail - (u8 *) head - sizeof(block_t),
            .id = HEAP_FIRST_UID
    };
    *tail = (block_t) {
            .prev = free,
            .next = NULL,
            .free = 0,
            .size = 0,
            .id = HEAP_TAIL_UID
    };

#ifdef DEBUG
    print(SERIAL, "warning Heap Address: %p\n"
                  "warning Heap size total %p\n"
                  "warning Heap size available %p\n\n", heap->heap_start, HEAP_SIZE, free->size);
#endif

    print(SERIAL, "info Heap initialized\n\n");
}
