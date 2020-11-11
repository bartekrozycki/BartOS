#pragma once

#include "ints.h"

typedef enum PageStatus {
    FREE = 0,
    ALLOCATED,
    SYSTEM,
    HOLE
} PageStatus;

void bitmap_set(u32 n, PageStatus status);
PageStatus bitmap_get(u32 n);