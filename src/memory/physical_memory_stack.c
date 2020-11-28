#include "physical_memory_stack.h"
#include "kernel_panic.h"

static u32 * stack_start;
static u32 * stackpointer;

void setMStackAdress(u32 *address)
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
    if (stackpointer == stack_start) kPanic;
    
    return (u32 *) *(stackpointer--);
}
u32 ms_freePages()
{
    return (stackpointer - stack_start);
}
