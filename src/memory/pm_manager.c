#include "mmu.h"
#include "system.h"

// TODO Defragmentation

static pm_queue pm;
static pm_node entry[1024]; // TODO kernel going to crash when entry limit reached
// static u32 iterator;

void init_pm(void)
{
    pm.head = entry;
    pm.tail = entry;
    pm.iterator = 0;
}

u32 pm_queue_push(u32 base, u32 length)
{
    if (pm.iterator >= 1024)
        kernel_panic(TERMINAL, "PMManager out of memory :(");
        
    (entry + pm.iterator)->base = base;
    (entry + pm.iterator)->size = length;
    (entry + pm.iterator)->next = (void *)0;

    if (pm.iterator == 0)
    {
        pm.head = entry + pm.iterator;
        pm.tail = pm.head;
    }
    else 
    {
        (entry + pm.iterator)->next = (pm_node *) pm.head;
        pm.head = entry + pm.iterator;
    }

    ++(pm.iterator);
    return 0x0;
}

// pm_node * pm_queue_pop(void) !!! should be used only when the entry is EMPTY !!! 
//                              !!! eg. pm_node->length reach 0                 !!!
pm_node* pm_queue_pop(void)
{
    pm_node *ptr = pm.head;
    pm.head = (pm_node *) pm.head->next;
    (pm.iterator)--;

    return ptr;
}

pm_node* pm_queue_top(void)
{
    return pm.head;
}
pm_node* pm_queue_bottom(void)
{
    return pm.tail;
}

u32 pm_node_avaible_space(pm_node *ptr)
{
    return ptr->size;
}