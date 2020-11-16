#pragma once

#include "boot_memory_init.h"
#include "ints.h"

typedef struct memory_stack
{
    u32 *address;
    u32 *next;
}memory_stack;

void init_stack(u32* kernel_address);
void ms_push(u32* address);
u32* ms_pop();
u32 ms_freePages();
