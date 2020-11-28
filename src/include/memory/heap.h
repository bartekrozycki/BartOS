#pragma once

#include "paging.h"

#define HEAP_FREE 1
#define HEAP_ALLOCATED 0

typedef struct block_t block_t;
typedef struct heap_t {
    block_t *head;
    block_t *tail;
    void *heap_start;
} heap_t;
struct block_t {
    block_t *p_next;
    block_t *p_prev;
    u32 size;
    i16 free; // 1-wolny; 0-zajęty
};

void init_heap();
heap_t* getHeap();
