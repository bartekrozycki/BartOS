#include "kernel_heap_malloc.h"
#include "kernel_heap.h"
#include "kernel_panic.h"
#include "print.h"

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
                .free = BLOCK_FREE,
                .size = p_new->size - size - sizeof(block_t),
                .p_next = p_next,
                .p_prev = p_new
        };
        *p_new = (block_t) {
                .free = BLOCK_ALLOCATED,
                .size = size,
                .p_next = p_rest
        };
        p_next->p_prev = p_rest;
    } else p_new->free = BLOCK_ALLOCATED;

    return ((u8 *) p_new + sizeof(block_t));
}

block_t *find_worst_fit(heap_t *heap, size_t min_size) {

    block_t *p_largest = heap->head->p_next;

    for (block_t *p_current = p_largest->p_next; p_current != heap->tail; p_current = p_current->p_next) {
        if (p_current->free == BLOCK_ALLOCATED)
            continue;

        if (p_current->size >= p_largest->size)
            p_largest = p_current;
    }

    if (p_largest->free == BLOCK_ALLOCATED || p_largest->size < min_size)
        return NULL; //out of memory

    return p_largest;
}

void heap_block_join(block_t *p_ref) {
    p_ref->free = BLOCK_FREE;
    p_ref->size = p_ref->size + sizeof(block_t) + p_ref->p_next->size;

    block_t *p_last = p_ref->p_next->p_next;

    p_ref->p_next = p_last;
    p_last->p_prev = p_ref;
}

void free(void *ptr) {
    if (!ptr) return;

    heap_t *heap = getHeap();
    block_t *p_block = (block_t *) ((u8 *) ptr - sizeof(block_t));

    int found = 0;

    for (const block_t *p = heap->head; p != heap->tail; p = p->p_next) {
        if (p == p_block) {
            found = 1;
            break;
        }
    }

    if (!found) return;    // TODO heap block not found

    p_block->free = BLOCK_FREE;

    print(SERIAL, "\n\nFREE: %p + 0xF\n", p_block);
    print(SERIAL, "Next: %p\n", p_block->p_next);
    print(SERIAL, "Prev: %p\n\n", p_block->p_prev);


    if (p_block->p_next && p_block->p_next->free)
        heap_block_join(p_block);
    if (p_block->p_prev && p_block->p_prev->free)
        heap_block_join(p_block->p_prev);

}
