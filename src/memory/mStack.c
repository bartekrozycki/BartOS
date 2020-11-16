// #include "mStack.h"
// #include "memory_manager.h"
// #include "system.h"

// extern MManager Manager;

// void ms_push(u32* address)
// {
//     if (Manager.Stack.ptr <= Manager.Stack.address)
//         permahalt();

//     memory_stack *next = Manager.Stack.ptr - sizeof(memory_stack);
//     next->address = address;
//     next->next = (u32 *)Manager.Stack.ptr;

//     ++Manager.Stack.free;
//     Manager.Stack.ptr = next;
// }

// u32* ms_pop()
// {
//     if (!Manager.Stack.free)
//         permahalt(); // TODO out of memory

//     u32 *addr = Manager.Stack.ptr->address;
//     Manager.Stack.ptr += sizeof(memory_stack);
//     --Manager.Stack.free;
//     return addr;
// }
