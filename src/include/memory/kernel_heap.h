#pragma once

#include "paging.h"

#define BLOCK_FREE 1
#define BLOCK_ALLOCATED 0

typedef struct block_t block_t;
typedef struct heap_t {
    void    *heap_start;
    block_t *head;
    block_t *tail;
} heap_t;
struct block_t {
    block_t *p_prev;
    u32 size;
    u32 free; // 1-wolny; 0-zajęty
    block_t *p_next;
};

void init_heap();
heap_t* getHeap();
void heap_block_join(block_t *p_ref);
