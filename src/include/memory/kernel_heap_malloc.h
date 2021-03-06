#pragma once

#include "kernel_heap.h"
#include "ints.h"

#define ALIGNED 8
#define ALIGN_SIZE(size) ((size + ALIGNED - 1) & ~(ALIGNED - 1))

void *malloc(size_t size);
void *calloc(size_t num, size_t size);
void free(void *ptr);
#ifdef DEBUG
void heap_debug();
#endif