#include "paging.h"
#include "lib.h"
#include "ints.h"
#include "print.h"
#include "system.h"

#define PRESENT 0b1

extern PageDirectory page_directory[1024];

/*
    Boot.asm map

    page_entry[1023] = kernel
    page_entry[1022] = low_memory
*/
extern PageTableEntry page_entry[1024];

static u32            page_entry_iter = 0;

void init_paging(void)
{
    ASSERT(PageDirectory, 4);
    ASSERT(PageTableEntry, 4);

    PageTableEntry *ptr;

    for (size_t i = 0; i < 1024; i++)
    {
        if ((page_directory + i)->present)
        { 
            print(SERIAL, "%d: %p\n", i, (page_directory + i)->entry);

            ptr = ((page_directory + i)->address << 12);

            for (size_t j = 0; j < 1024; j++)
                if((ptr + j)->present)
                    print(SERIAL, "%d.%d: %p\n", i, j, (ptr + j)->address << 12);
        }
    }   
}

void map_page(void * physaddr, void * virtualaddr, unsigned int flags)
{
   // Make sure that both addresses are page-aligned.
    if (((u32)physaddr & 0xFFF) || ((u32)virtualaddr & 0xFFF))
        return;

    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
    
    if (!page_directory[pdindex].present)
        page_directory[pdindex].entry = ((u32) &(page_entry[page_entry_iter++])) | 0x1;
 
    // Here you need to check whether the PT entry is present.
    // When it is, then there is already a mapping present. What do you do now?

    PageTableEntry *pt = (PageTableEntry *) (page_directory[pdindex].address);
    if (pt->present)
    {
        print(TERMINAL, "#@!#!@   %p   $!@#@!#@!\n", virtualaddr);
        kernel_panic(TERMINAL, "PTE address is already mapped !!!!");
    }
 
    // pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Present
 
    // Now you need to flush the entry in the TLB
    // or you might not notice the change.
}