#include "memory_manager.h"

#define FRAMES_COUNT (Manager.Memory.HighestAddress >> 12)

PageDirectory directory[1024];
MManager Manager;

u32 init_memory_manager(MultibootInfo* mbi)
{
    if (!(mbi->flags & MULTIBOOT_FLAG_MMAP))
        return 1;
    
    // Initalized MManager values :)

    Manager.Memory.KernelBootAddress = 0x100000;
    Manager.Memory.KernelEndAddress  = (u32) &_kernel_end - 0xE0000000;
    Manager.Memory.KernelHighAddress = 0xE0000000;
    Manager.Memory.HighestAddress    = get_highest_adrress(mbi);
    
    Manager.Pagging.PD_address = directory;

    // Generate bitmap of physical memory :o
    init_bitmap_space();
    init_stack_space();

    return 0;
}

u32 get_highest_adrress(MultibootInfo* mbi)
{
    u32 h;
    for (MultibootMemoryMap *mmap = (MultibootMemoryMap *) mbi->mmap_address; (u32) mmap < (mbi->mmap_address + mbi->mmap_length); mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size)))
        if (mmap->type == MULTIBOOT_MMAP_FREE_MEMORY)
            h = mmap->baselow + mmap->lenlow;
 
    return h;
}

// 1 page ==> 2 bits  |  1 byte can store 4 pages info
void init_bitmap_space()
{
    Manager.Bitmap.size_in_bytes = (FRAMES_COUNT >> 2); // div 0x4
    Manager.Bitmap.address = (Manager.Memory.KernelEndAddress);

    // clear
    for (u8* i = (u8 *) Manager.Memory.KernelEndAddress, 
             *j = (u8 *) (Manager.Memory.KernelEndAddress + Manager.Bitmap.size_in_bytes); i < j; i++)
        *i = 0;    

    Manager.Memory.KernelEndAddress += Manager.Bitmap.size_in_bytes;
}
void init_stack_space()
{
    Manager.Stack.size_in_bytes = (FRAMES_COUNT << 5); // mul 32
    Manager.Stack.address = (Manager.Memory.KernelEndAddress);

    // clear
    for (u8* i = (u8 *) Manager.Memory.KernelEndAddress, 
             *j = (u8 *) (Manager.Memory.KernelEndAddress + Manager.Stack.size_in_bytes); i < j; i++)
        *i = 0;   

    Manager.Memory.KernelEndAddress += Manager.Bitmap.size_in_bytes;
}

void enablePaging(PageDirectory* pd_addr)
{
    __asm__ ("mov %0, %%cr3": : "a"(pd_addr));
    __asm__ ("mov %cr0, %eax ");
    __asm__ ("or %eax, 0x80000000");
    __asm__ ("mov %eax, %cr0");
}
