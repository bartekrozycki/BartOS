#pragma once

#include "boot_memory_init.h"
#include "ints.h"

typedef struct memory_stack
{
    u32 *address;
    u32 *next;
}memory_stack;

void setMStackAdress(u32 *address);

void init_stack(u32* stack_memory_space);
void ms_push(u32* address);
u32* ms_pop();
u32 ms_freePages();
