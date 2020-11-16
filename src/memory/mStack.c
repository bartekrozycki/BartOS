#include "mStack.h"

static u32 * stack_start;
static u32 * stackpointer;
/**
 * @param address Address of bitmap
 */
void init_stack(u32* address)
{
    stack_start = address;
    stackpointer = address;
}

void ms_push(u32* address)
{
    *++stackpointer = (u32) address;
}

u32* ms_pop()
{
    if (stackpointer == stack_start) return 0x0;
    return (u32 *) *(stackpointer--);
}
u32 ms_freePages()
{
    return (stackpointer - stack_start);
}
