#include "paging.h"
#include "kernel_panic.h"
#include "mem_map.h"
#include "interrupts_handlers.h"
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

static void pageFault(InterruptSave *is) {
    u32 *cr2_value = (u32 *) 0xDEADBEEF;
    __asm__ ("movl %%cr2, %0": "=r"(cr2_value) : );

    print(SERIAL, "\n---====[ Detected page fault.\n");
    print(SERIAL, "Error code: %b\nError Message: %s\nInfo CR2: %p\n", is->err, page_fault_messages[is->err],
          cr2_value);
    print(SERIAL,
          "Info EAX %p\nInfo EBX %p\nInfo ECX %p\nInfo EDX %p\nInfo ESP %p\nInfo EBP %p\nInfo ESI %p\nInfo EDI %p\n",
          is->eax, is->ebx, is->ecx, is->edx, is->esp, is->ebp, is->esi, is->edi);
    print(SERIAL, "Info EIP %p\nInfo CS %p\nInfo EFLAGS %p\nInfo USER_ESP %p\nInfo SS %p\n",
          is->eip, is->cs, is->eflags, is->useresp, is->ss);
    print(SERIAL, "]====---\n");

#ifdef DEBUG
    BOCHS_BREAK;
#endif
}

void init_paging() {
    isr_new_call(14, pageFault);


    //clear first 4 MiB;
    PageTableEntry *entry = (PageTableEntry *) (directory[0].address << 12);
    memset(entry, ' ', 1024 * sizeof(PageTableEntry));
    directory[0].present = 0;

    // reload PD 
    u32 reset;
    __asm__ ("movl %%cr3, %0": "=r"(reset):);
    __asm__ ("movl %0, %%cr3": : "a"(reset));
    // end reload PD


}

// Source: https://wiki.osdev.org/Inline_Assembly/Examples
static inline void invlpg(const void *m) {
    __asm__( "invlpg (%0)" : : "b"(m) : "memory" );
}

u32 getPhysicalAddress(u32 virtual_address) {
    if (((u32) virtual_address & 0xFFF))
        kPanic;

    u32 pd_index = (u32) virtual_address >> 22;
    u32 pt_index = (u32) virtual_address >> 12 & 0x03FF;

    if (!directory[pd_index].present) return 0x0;

    PageTableEntry *entry = (PageTableEntry *) (directory[pd_index].address << 12);
    BOCHS_BREAK;
    if (!entry[pt_index].present) return 0x0;

    return (u32) (entry[pt_index].address << 12);
}

void remap(u32 virtual_old, u32 virtual_new) {
    if (((u32) virtual_old & 0xFFF) || ((u32) virtual_new & 0xFFF))
        kPanic;

    u32 old_pd = (u32) virtual_old >> 22;
    u32 old_pt = (u32) virtual_old >> 12 & 0x03FF;
    u32 new_pd = (u32) virtual_new >> 22;
    u32 new_pt = (u32) virtual_new >> 12 & 0x03FF;

    if (!directory[old_pd].present) permahalt();
    PageTableEntry *old_entry = (PageTableEntry *) (directory[old_pd].address << 12);
    if (!old_entry[old_pt].present) permahalt();

    u32 entry = old_entry[old_pt].address << 12;

    if (!directory[new_pd].present)
        directory[new_pd].present = 1;

    PageTableEntry *new_entry = (PageTableEntry *) (directory[new_pd].address << 12);

    new_entry[new_pt].entry = entry | 0x1;
    invlpg((const u32 *) virtual_new);
}

void map(u32 physical_address, u32 virtual_address) {
    if (((u32) physical_address & 0xFFF) || ((u32) virtual_address & 0xFFF))
        kPanic;

    u32 pd_index = (u32) virtual_address >> 22;
    u32 pt_index = (u32) virtual_address >> 12 & 0x03FF;

    if (!(directory[pd_index].present))
        (directory[pd_index].present) = 1;
    invlpg((const u32 *) &directory[pd_index]);

    PageTableEntry *pt = (PageTableEntry *) (directory[pd_index].address << 12);

    pt[pt_index].entry = ((u32) physical_address) | 0x01; // Present
    invlpg((const u32 *) virtual_address);
}