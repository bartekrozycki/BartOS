#include "kernel_heap_malloc.h"
#include "heap.h"
#include "kernel_panic.h"

block_t *find_worst_fit(heap_t *heap, size_t min_size);

void *malloc(size_t size) {
    heap_t *heap;
    if ((heap = getHeap()) == NULL)
        kPanic;

    size = ALIGN_SIZE(size);
    block_t *p_new = find_worst_fit(heap, size);

    if (p_new == NULL)
        return NULL; // OUT OF MEMORY

    if (p_new->size >= (size + sizeof(block_t))) {
        block_t *p_rest = (block_t *) ((u8 *) p_new + sizeof(block_t) + size);
        block_t *p_next = p_new->p_next;

        *p_rest = (block_t) {
                .free = HEAP_FREE,
                .size = p_new->size - size - sizeof(block_t),
                .p_next = p_next,
                .p_prev = p_new
        };
        *p_new = (block_t) {
                .free = HEAP_ALLOCATED,
                .size = size,
                .p_next = p_rest
        };
        p_next->p_prev = p_rest;
    } else p_new->free = HEAP_ALLOCATED;

    return ((u8*)p_new + sizeof(block_t));
}

block_t *find_worst_fit(heap_t *heap, size_t min_size) {

    block_t *p_largest = heap->head->p_next;

    for (block_t *p_current = p_largest->p_next; p_current != heap->tail; p_current = p_current->p_next) {
        if (p_current->free == HEAP_ALLOCATED)
            continue;

        if (p_current->size >= p_largest->size)
            p_largest = p_current;
    }

    if (p_largest->free == HEAP_ALLOCATED || p_largest->size < min_size)
        return NULL; //out of memory

    return p_largest;
}

void free(void *ptr) {
    // TODO free
}
