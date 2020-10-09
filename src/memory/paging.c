#include "paging.h"
#include "lib.h"
#include "print.h"

#define PRESENT 0b1


void init_paging(void)
{
    ASSERT(PageDirectoryEntry, 4);
    ASSERT(PageTableEntry, 4);
}