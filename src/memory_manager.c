#include "memory_manager.h"
#include "bitmap.h"

#define KERNEL_BOOT_VMA 0x00100000
#define KERNEL_HIGH_VMA 0xE0000000

#define FRAMES_COUNT (Manager.Memory.HighestAddress >> 12)
#define ALIGN_TO(x,mask) (((x) + (mask) - 1) & ~((mask) - 1))

MManager Manager;

/**
 * @param mbi Multiboot structure address
 * @return 0 if everythink is ok
 */
u32 init_memory_manager(MultibootInfo* mbi)
{
    if (!(mbi->flags & MULTIBOOT_FLAG_MMAP))
        return 1;
    
    Manager.Memory.KernelEndAddress  = (u32) &_kernel_end - KERNEL_HIGH_VMA;
    Manager.Memory.HighestAddress    = get_highest_adrress(mbi);
    
    Manager.Bitmap.size_in_bytes = (FRAMES_COUNT >> 2); // div 0x4
    Manager.Bitmap.address = ke_alloc(Manager.Bitmap.size_in_bytes);
    
    Manager.Stack.size_in_bytes = (FRAMES_COUNT << 5); // mul 32
    Manager.Stack.address = (memory_stack *) ke_alloc(Manager.Stack.size_in_bytes);
    Manager.Stack.ptr = (Manager.Stack.address + Manager.Stack.size_in_bytes - sizeof(memory_stack));
    Manager.Stack.free = 0;

    Manager.Pagging.PD_address = (PageDirectory *) ke_alloc(sizeof(PageDirectory) * 1024);

    initial_kernel_paging(); 

    //pagin enabled from now, so now i can use functionality from higher kernel
    
    return 0;
}

/**
 * @param mbi Multiboot structure address
 * @return Highest physical address avaible in memory
 */
u32 get_highest_adrress(MultibootInfo* mbi)
{
    u32 h;
    for (MultibootMemoryMap *mmap = (MultibootMemoryMap *) mbi->mmap_address; (u32) mmap < (mbi->mmap_address + mbi->mmap_length); mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size)))
        if (mmap->type == MULTIBOOT_MMAP_FREE_MEMORY)
            h = mmap->baselow + mmap->lenlow;
 
    return h;
}

/**
 * Space after kernel end, all values in created block is 0, aligned to 0x1000
 * @param size length in bytes u need
 * @return address of block
 */
u8* ke_alloc(u32 size)
{
    u8 *addr = (u8 *) Manager.Memory.KernelEndAddress;
    size = ALIGN_TO(size, 0x1000);
    // 000000
    for (u8* i = (u8 *)addr, 
            *j = (u8 *)(addr + size); i < j; i++) *i = 0;  

    Manager.Memory.KernelEndAddress += size;
    return addr;
}

/**
 * Sets CR3 to given address
 * Sets CR0 to enable paging
 * @param pd_addr Paging PageDirectory address
 */
void enablePaging(PageDirectory* pd_addr)
{
    __asm__ ("movl %0, %%cr3": : "a"(pd_addr));
    __asm__ ("movl %cr0, %eax ");
    __asm__ ("movl $0x80000000, %ebx");
    __asm__ ("orl %ebx, %eax"); // 1 << 31
    __asm__ ("movl %eax, %cr0");
}

/**
 * Identy maping from 0x0 to KERNEL_END
 * Maping KERNEL_HIGH_VMA ---> KERNEL_BOOT_VMA
 * 
 * Function enable paging using enablePaging()
 */
void initial_kernel_paging()
{
    u32 paging_required_size_in_bytes = Manager.Memory.KernelEndAddress >> 10; // div 0x1000 mul 0x4
    PageTableEntry *tab = (PageTableEntry *) ke_alloc(paging_required_size_in_bytes);
    
    u32 allocated_pages = ALIGN_TO(paging_required_size_in_bytes, 0x1000) >> 12; // div 0x1000

    for (u32 memory_ptr = 0x0, i = 0; 
        memory_ptr < Manager.Memory.KernelEndAddress; 
        memory_ptr += 0x1000, i++) 
    {
        (tab + i)->entry = memory_ptr | 0x1; // present
    }

    for (u32 i = 0; i < allocated_pages; i++)
        Manager.Pagging.PD_address[i].entry = ((u32) (tab + (0x1000 * i))) | 0x1;
    
    // map high memory

    paging_required_size_in_bytes = (Manager.Memory.KernelEndAddress - KERNEL_BOOT_VMA) >> 10;
    tab = (PageTableEntry *) ke_alloc(paging_required_size_in_bytes);
    allocated_pages = ALIGN_TO(paging_required_size_in_bytes, 0x1000) >> 12;

    for (u32 memory_ptr = KERNEL_BOOT_VMA, i = (KERNEL_BOOT_VMA >> 12); 
        memory_ptr < Manager.Memory.KernelEndAddress; 
        memory_ptr += 0x1000, i++) 
    {
        (tab + i)->entry = memory_ptr | 0x1; // present
    }
    for (u32 i = 0; i < allocated_pages; i++)
        Manager.Pagging.PD_address[(KERNEL_HIGH_VMA >> 22) + i].entry = ((u32) (tab + (0x1000 * i))) | 0x1;

    enablePaging(Manager.Pagging.PD_address);
}

void init_physical(MultibootInfo* mbi)
{
    
}