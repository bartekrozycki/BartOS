#include "mStack.h"
#include "memory_manager.h"
#include "system.h"

extern MManager Manager;

/**
 * @param address free frame address
 */
void ms_push(u32* address)
{
    if (Manager.Stack.ptr <= Manager.Stack.address)
        kernel_panic(SERIAL, "Memory stack overflowed"); // TODO stack overflow

    memory_stack *next = Manager.Stack.ptr - 1;
    next->address = address;
    next->next = (u32 *)Manager.Stack.ptr;

    ++Manager.Stack.free;
    Manager.Stack.ptr = next;
}
/**
 * @return address of free frame
 */
u32* ms_pop()
{
    if (!Manager.Stack.free)
        kernel_panic(SERIAL, "Out of memory"); // TODO out of memory

    u32 *addr = Manager.Stack.ptr->address;
    Manager.Stack.ptr += 1;
    --Manager.Stack.free;
    return addr;
}