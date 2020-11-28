#pragma once

#include "heap.h"
#include "../std/ints.h"

#define ALIGNED 8
#define ALIGN_SIZE(size) ((size + ALIGNED - 1) & ~(ALIGNED - 1))

void *malloc(size_t size);
void free(void *ptr);
