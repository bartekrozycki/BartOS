#include "mStack.h"
#include "memory_manager.h"
#include "system.h"

extern MManager m;

void ms_push(u32* address)
{
    if (m.Stack.ptr <= m.Stack.address)
        permahalt();

    memory_stack *next = m.Stack.ptr - sizeof(memory_stack);
    next->address = address;
    next->next = (u32 *)m.Stack.ptr;

    ++m.Stack.free;
    m.Stack.ptr = next;
}

u32* ms_pop()
{
    if (!m.Stack.free)
        permahalt(); // TODO out of memory

    u32 *addr = m.Stack.ptr->address;
    m.Stack.ptr += sizeof(memory_stack);
    --m.Stack.free;
    return addr;
}