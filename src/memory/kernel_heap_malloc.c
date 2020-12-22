#include "kernel_heap_malloc.h"
#include "kernel_heap.h"
#include "kernel_panic.h"
#include "print.h"

block_t *find_worst_fit(heap_t *heap, size_t min_size);
#ifdef DEBUG
void heap_debug()
{
    heap_t *heap = getHeap();
    print(SERIAL, "\n\nHeap: %p\n", heap);
    print(SERIAL, "Head: %p    head->next %p\n", heap->head, heap->head->p_next);
    print(SERIAL, "Tail: %p    tail->prev %p\n", heap->tail, heap->tail->p_prev);

    block_t *block = heap->head->p_next;
    for (int i = 0; 1; ++i) {
        print(SERIAL, "BLOCK #%d %p   next %p    prev %p    size %p    status %d\n", i, block, block->p_next, block->p_prev, block->size, block->free);
        block = block->p_next;
        if (block == heap->tail) break;
    }
}
#endif
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

        block_t *temp_prev = p_new->p_prev;

        *p_rest = (block_t) {
                .free = BLOCK_FREE,
                .size = p_new->size - size - sizeof(block_t),
                .p_next = p_next,
                .p_prev = p_new
        };
        *p_new = (block_t) {
                .free = BLOCK_ALLOCATED,
                .size = size,
                .p_next = p_rest,
                .p_prev = temp_prev
        };
        p_next->p_prev = p_rest;
    } else p_new->free = BLOCK_ALLOCATED;
#ifdef DEBUG_1
    heap_debug();
#endif
    return ((u8 *) p_new + sizeof(block_t));
}
void *calloc(size_t num, size_t size)
{
    size_t total = num * size;
    u8* ptr = malloc(total);
    if (!ptr) return NULL;
    for (size_t i = 0; i < total; i++)
        *(ptr + i) = 0;
    return ptr;
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

    if (p_block->p_next->free)
        heap_block_join(p_block);
    if (p_block->p_prev->free)
        heap_block_join(p_block->p_prev);

}
