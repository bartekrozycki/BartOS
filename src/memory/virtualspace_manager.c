#include "paging.h"
#include "kernel_panic.h"
#include "boot_memory_init.h"
#include "mem_map.h"
#include "kalloc.h"
#include "IRQ_handlers.h"
#include "print.h"
#include "lib.h"

PageDirectory *directory = (PageDirectory *) KERNEL_STRUCTURES_SPACE;

const char *page_fault_messages[8] = {
    "Supervisory process tried to read a non-present page entry",
    "Supervisory process tried to read a page and caused a protection fault",
    "Supervisory process tried to write to a non-present page entry",
    "Supervisory process tried to write a page and caused a protection fault",
    "User process tried to read a non-present page entry",
    "User process tried to read a page and caused a protection fault",
    "User process tried to write to a non-present page entry",
    "User process tried to write a page and caused a protection fault"
};

static void pageFault(InterruptSave *is)
{
    u32 *cr2_value = 0xDEADBEEF;
    __asm__ ("movl %%cr2, %0": "=r"(cr2_value) : );

    print(SERIAL, "\n---====[ Detected page fault.\n");
    print(SERIAL, "Error code: %b\nError Message: %s\nInfo CR2: %p\n", is->err, page_fault_messages[is->err], cr2_value);
    print(SERIAL, "Info EAX %p\nInfo EBX %p\nInfo ECX %p\nInfo EDX %p\nInfo ESP %p\nInfo EBP %p\nInfo ESI %p\nInfo EDI %p\n", 
                    is->eax, is->ebx, is->ecx, is->edx, is->esp, is->ebp, is->esi, is->edi);
    print(SERIAL, "Info EIP %p\nInfo CS %p\nInfo EFLAGS %p\nInfo USER_ESP %p\nInfo SS %p\n", 
                    is->eip, is->cs, is->eflags, is->useresp, is->ss);
    print(SERIAL, "]====---\n");
}

void init_paging()
{
    isr_new_call(14, pageFault);


    //clear first 4 MiB;
    PageTableEntry *entry = (PageTableEntry *) (directory[0].address << 12);
    memset(entry, ' ', 1024 * sizeof(PageTableEntry));
    directory[0].present = 0;

    // reload PD 
    u32 reset;
    __asm__ ("movl %%cr3, %0": "=r"(reset):);
    __asm__ ("movl %0, %%cr3": : "a"(reset));
    ///


    u32 pd_index = KERNEL_BOOT_VMA >> 22;
}


// Source: https://wiki.osdev.org/Inline_Assembly/Examples
static inline void invlpg(const void* m)
{
    __asm__( "invlpg (%0)" : : "b"(m) : "memory" );
}

u32 getPhysicalAddress(u32 virtualaddr)
{
    if (((u32)virtualaddr & 0xFFF))
        kPanic;

    u32 pdindex = (u32) virtualaddr >> 22;
    u32 ptindex = (u32) virtualaddr >> 12 & 0x03FF;

    if (!directory[pdindex].present) return 0x0;

    PageTableEntry *entry = (PageTableEntry *) (directory[pdindex].address << 12);
    BOCHS_BREAK;
    if (!entry[ptindex].present) return 0x0;

    return (u32) (entry[ptindex].address << 12);
}
void remap(u32 virtual_old, u32 virtual_new)
{
    if (((u32)virtual_old & 0xFFF) || ((u32)virtual_new & 0xFFF))
        kPanic;

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

    newentry[new_pt].entry = entry | 0x1;
    invlpg((const u32 *)virtual_new);    
}
void map(u32 physaddr, u32 virtualaddr)
{
    if (((u32)physaddr & 0xFFF) || ((u32)virtualaddr & 0xFFF))
        kPanic;

    u32 pdindex = (u32) virtualaddr >> 22;
    u32 ptindex = (u32) virtualaddr >> 12 & 0x03FF;
    
    if (!(directory[pdindex].present))
        (directory[pdindex].present) = 1; // TODO temporary
    
    PageTableEntry *pt = (PageTableEntry *)(directory[pdindex].address << 12);

    pt[ptindex].entry = ((u32)physaddr) | 0x01; // Present
    invlpg((const u32 *)virtualaddr);    
}