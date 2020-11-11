#pragma once

#include "ints.h"

typedef struct memory_stack
{
    u32 *address;
    u32 *next;
}memory_stack;

void ms_push(u32* address);
u32* ms_pop();