#include "paging.h"
#include "lib.h"
#include "ints.h"
#include "print.h"
#include "system.h"
#include "mem.h"

#define PRESENT 0b1

extern PageDirectory page_directory[1024];
extern PageTableEntry page_entry_low_memory[1024];
extern PageTableEntry page_entry_kernel[1024];

static inline void invlpg(const void* m)
{
    __asm__( "invlpg (%0)" : : "b"(m) : "memory" );
}

void init_paging(void)
{
    ASSERT(PageDirectory, 4);
    ASSERT(PageTableEntry, 4);
    
    // for (unsigned int i = 0; i < 1024; i++)
    // {
    //     PageTableEntry *ptr;
    //     if ( ptr = P_Get_TableEntry(i) )
    //     {
    //         for (unsigned int j = 0; j < 1024; j++)
    //         {
    //             print(SERIAL, "[%d . %d] > %p\n", i, j, (ptr + j)->address << 12 );
    //         }
            
    //     }
    // }
}

void P_Directory_Set_Entry(u32 index, const u32 *entry)
{
    if (index >= 1024 || (u32) entry & 0xFFF)
        kernel_panic(TERMINAL, "P_Directory_Set_Entry - > kernel panic.");

    page_directory[index].entry = (u32) entry | 0x1;
}
u32 P_Directory_Get_Entry(u32 index)
{
    if (index >= 1024)
        kernel_panic(TERMINAL, "P_Directory_Get_Entry - > kernel panic.");

    return (u32) page_directory[index].entry;
}
PageTableEntry * P_Get_TableEntry(u32 pd_index)
{
    if (pd_index >= 1024)
        kernel_panic(TERMINAL, "P_Get_TableEntry - > kernel panic.");
    if (!page_directory[pd_index].present) return NULL;

    // print(TERMINAL, "\n%p", ( page_directory[pd_index].address << 12));

    return (PageTableEntry *) (page_directory[pd_index].address << 12);
}

PageTableEntry* exception_pageentry_notfound(void)
{
    PageTableEntry *ptr = _kalloc_frame(1); // 0x1000 frame size
    if (!ptr) // handle out of memory
        kernel_panic(TERMINAL, "--------> exception_pageentry_notfound # out of memory");

    return ptr;
}

void map_page(void * physaddr, void * virtualaddr, unsigned int flags)
{
   // Make sure that both addresses are page-aligned.
    if (((u32)physaddr & 0xFFF) || ((u32)virtualaddr & 0xFFF))
        return;

    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
    
    if (!page_directory[pdindex].present)
        page_directory[pdindex].entry = ((u32) exception_pageentry_notfound() ) | 0x1;
 

    PageTableEntry *pt = (PageTableEntry *) (page_directory[pdindex].address << 12);

    if (pt[ptindex].present)
    {
        // print(SERIAL, "remapping: %p %p\n", physaddr, virtualaddr);
        // print(TERMINAL, "$!@#@!#@!   %p  \\\\  %p $!@#@!#@!\n", virtualaddr, pt[ptindex].entry);
        // kernel_panic(TERMINAL, "PTE address is already mapped PANIC :D!!!!");
    }
    // else print(SERIAL, "map: %p %p\n", physaddr, virtualaddr);
 
    pt[ptindex].entry = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Present
 
    invlpg(virtualaddr);
}