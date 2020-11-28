#pragma once

#include "paging.h"

#define NULL ((void *) 0)

#define HEAP_HEAD_UID -1
#define HEAP_TAIL_UID -2
#define HEAP_FIRST_UID 1

#define HEAP_FREE 1
#define HEAP_ALLOCATED 0

typedef struct block_t block_t;
typedef struct heap_t {
    block_t *head;
    block_t *tail;
    void *heap_start;
    u16 current_id;
    u16 unused;
} heap_t;
struct block_t {
    block_t *next;
    block_t *prev;
    i32 size;
    i16 free; // 1-wolny; 0-zajęty
    i16 id;
};

void init_heap();
