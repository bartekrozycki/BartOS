#pragma once

#include "multiboot.h"
#include "paging.h"
#include "ints.h"

#define PAGE(n) ((n) >> 12)

void setBitmapAddress(u32 *address);

typedef enum PageStatus {
    FREE = 0,
    ALLOCATED,
    SYSTEM,
    HOLE
} PageStatus;

void init_bitmap(u32* address);

void bitmap_set(u32 n, PageStatus status);
PageStatus bitmap_get(u32 n);