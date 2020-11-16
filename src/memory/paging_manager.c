#include "paging.h"
#include "system.h"
#include "boot_memory_init.h"
#include "mem_map.h"
#include "kalloc.h"

static PageDirectory *directory;

static inline void invlpg(const void* m)
{
    __asm__( "invlpg (%0)" : : "b"(m) : "memory" );
}

void init_paging(PageDirectory *pd)
{
    directory = pd;

    remap((u32)directory, KERNEL_STRUCTURES_SPACE);
    __asm__("xchgw %bx, %bx");
    directory = (PageDirectory *) KERNEL_STRUCTURES_SPACE;
    for (u32 i = 0; i < 1024; i++)
        remap((u32)(directory[i].address << 12), KERNEL_STRUCTURES_SPACE + (0x1000 * i + 0x1000));
    
    __asm__("xchgw %bx, %bx");

}
u32 getPhysicalAddress(u32 virtualaddr)
{
    if (((u32)virtualaddr & 0xFFF))
        permahalt();

    u32 pdindex = (u32) virtualaddr >> 22;
    u32 ptindex = (u32) virtualaddr >> 12 & 0x03FF;

    if (!directory[pdindex].present) return 0x0;

    PageTableEntry *entry = (PageTableEntry *) (directory[pdindex].address << 12);
    if (!entry[ptindex].present) return 0x0;

    return (u32) (entry[ptindex].address << 12);
}
void remap(u32 virtual_old, u32 virtual_new)
{
    if (((u32)virtual_old & 0xFFF) || ((u32)virtual_new & 0xFFF))
        permahalt();

    u32 old_pd = (u32) virtual_old >> 22;
    u32 old_pt = (u32) virtual_old >> 12 & 0x03FF;
    u32 new_pd = (u32) virtual_new >> 22;
    u32 new_pt = (u32) virtual_new >> 12 & 0x03FF;

    if (!directory[old_pd].present) permahalt();
    PageTableEntry *oldentry = (PageTableEntry *) (directory[old_pd].address << 12);
    if (!oldentry[old_pt].present) permahalt();

    u32 entry = oldentry[old_pt].address << 12;

    if (!directory[new_pd].present)
        directory[new_pd].present = 1;

    PageTableEntry *newentry = (PageTableEntry *) (directory[new_pd].address << 12);
    
    print(SERIAL, "xd > %p\n", entry);

    newentry[new_pt].entry = entry | 0x1;
    invlpg(virtual_new);    
}
void map(u32 physaddr, u32 virtualaddr)
{
   // Make sure that both addresses are page-aligned.
    if (((u32)physaddr & 0xFFF) || ((u32)virtualaddr & 0xFFF))
        permahalt();

    u32 pdindex = (u32) virtualaddr >> 22;
    u32 ptindex = (u32) virtualaddr >> 12 & 0x03FF;
    
    if (!(directory[pdindex].present))
        permahalt();

    PageTableEntry *pt = (PageTableEntry *)(directory[pdindex].address << 12);

    pt[ptindex].entry = ((u32)physaddr) | 0x01; // Present
}