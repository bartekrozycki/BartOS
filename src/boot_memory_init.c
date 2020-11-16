#include "boot_memory_init.h"
#include "system.h"
#include "bitmap.h"
#include "terminal.h"
#include "print.h"
#include "heap.h"
#include "serial.h"


extern void Main(MultibootInfo* mboot_info);


/**
 * @param mbi Multiboot structure address
 * @return 0 if everythink is ok
 */
void boot_init_mem(u32 mboot_magic, MultibootInfo* mbi)
{
	if (mboot_magic != MULTIBOOT_EAX_MAGIC)
		permahalt();

    if (!(mbi->flags & MULTIBOOT_FLAG_MMAP))
        permahalt();
    

    u32 kernel_start        = (u32) &_kernel_start - KERNEL_HIGH_VMA;
    u32 kernel_end          = (u32) &_kernel_end - KERNEL_HIGH_VMA;
    u32 highest_address     = get_highest_adrress(mbi, &highest_address);
    u32 memory_bitmap_size  = FRAMES_COUNT(highest_address) / 4;
    u32 memory_stack_size   = FRAMES_COUNT(highest_address) * 4 + 4; // [AAAAxxxxxxx] (u32) stackpointer

    u32* bitmap     = (u32 *) ke_alloc(&kernel_end, memory_bitmap_size);
    clearSpace((u8*)bitmap, memory_bitmap_size);
    u32* stack      = (u32 *) ke_alloc(&kernel_end, memory_stack_size);
    clearSpace((u8*)stack, memory_stack_size);

    PageDirectory* page_dir   = (PageDirectory *) ke_alloc(&kernel_end, sizeof(PageDirectory) * 0x400);
    PageTableEntry* dir_entrys = (PageTableEntry *) ke_alloc(&kernel_end, sizeof(PageTableEntry) * 0x400 * 0x400);
    for (u32 i = 0; i < 0x400; ++i)
        page_dir[i].entry = (u32) (dir_entrys + (0x400 * i));

    clearSpace((u8*)dir_entrys, sizeof(PageTableEntry) * 0x400 * 0x400);

    // map memory phys-virt
    initial_kernel_paging(&kernel_end, page_dir, mbi); 

    init_serial();

    init_stack(stack); 
    init_bitmap(bitmap);
    init_memory(mbi, &kernel_start, &kernel_end);

    init_heap(&kernel_end, page_dir);

    init_paging(page_dir);    

    Main(mbi);
}

/**
 * @param mbi Multiboot structure address
 * @return Highest physical address avaible in memory
 */
u32 get_highest_adrress(MultibootInfo* mbi, u32 *ret)
{
    for (MultibootMemoryMap *mmap = (MultibootMemoryMap *) mbi->mmap_address; (u32) mmap < (mbi->mmap_address + mbi->mmap_length); mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size)))
        if (mmap->type == MULTIBOOT_MMAP_FREE_MEMORY)
            *ret = mmap->baselow + mmap->lenlow;
    return *ret;
}

/**
 * Space after kernel end, all values in created block is 0, aligned to 0x1000
 * @param size length in bytes u need
 * @return address of block
 */
u32 ke_alloc(u32* kernel_end, u32 size)
{
    if (*kernel_end & 0xFFF) permahalt();
    u32 addr = *kernel_end;
    size = ALIGN_TO(size, 0x1000);

    *kernel_end += size;

    if (*kernel_end & 0xFFF) permahalt();
    return addr;
}
void clearSpace(u8* address, u32 len)
{
    for (u32 i = 0; i < len ; i++)
        address[i] = 0;
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
 * @param ke Kernel end adderss
 * @param pd Page directory
 */
void initial_kernel_paging(u32 * ke, PageDirectory *pd, MultibootInfo *mbi)
{
    // map 1:1 pre-reserved space depends on multiboot memory map
    for (MultibootMemoryMap *mmap = (MultibootMemoryMap *) mbi->mmap_address;
        (u32) mmap < (mbi->mmap_address + mbi->mmap_length); 
        mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size)))
    {
        if (mmap->type == MULTIBOOT_MMAP_RESERVED && mmap->baselow < KERNEL_BOOT_VMA)
        {
            for (u32 i = 0x0; i < mmap->lenlow; i+= 0x1000)
            {
                kmap(pd, mmap->baselow + i, mmap->baselow + i);
            }
        }
    }
    kmap(pd, (u32) mbi, (u32) mbi); // kmap mbi

    for (u32 i = KERNEL_BOOT_VMA; i < (u32)*ke; i += 0x1000)
    {
        kmap(pd, i, i);
        kmap(pd, i, 0xE0000000 + i);
    }
    kmap(pd, 0xB8000, 0xB8000); // wideło
    
    enablePaging(pd);
}
/**
 * Do not use later :)
 * @param pd PageDirectory address
 * @param physaddr Physical address
 * @param virtualaddr Virtual address
 * @return permahalt() if PTE not exist or virtual address already mapped
 */
void kmap(PageDirectory * pd, u32 physaddr, u32 virtualaddr)
{
   // Make sure that both addresses are page-aligned.
    if (((u32)physaddr & 0xFFF) || ((u32)virtualaddr & 0xFFF))
        permahalt();

    u32 pdindex = (u32) virtualaddr >> 22;
    u32 ptindex = (u32)virtualaddr >> 12 & 0x03FF;
    
    if (!((PageDirectory *)pd)[pdindex].present)
        ((PageDirectory *)pd)[pdindex].present = 1;

    PageTableEntry *pt = (PageTableEntry *) (((PageDirectory *)pd)[pdindex].address << 12);

    pt[ptindex].entry = ((u32)physaddr) | 0x01; // Present
}
/**
 * @param mbi Multiboot Sctructure
 * @param kernel_start pointer to variable with kernel_start address
 * @param kernel_end pinter to variable with kernel_end address
 */
void init_memory(MultibootInfo *mbi, u32 *kernel_start, u32 *kernel_end)
{
    for (MultibootMemoryMap *mmap = (MultibootMemoryMap *) mbi->mmap_address;
    (u32) mmap < (mbi->mmap_address + mbi->mmap_length); 
    mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size)))
    {
        if (mmap->type == MULTIBOOT_MMAP_RESERVED && mmap->baselow < KERNEL_BOOT_VMA)
        {
            for (u32 i = 0x0; i < mmap->lenlow; i+= 0x1000)
            {
                bitmap_set(PAGE(i + mmap->baselow), SYSTEM); // reserved space known from multibootinfo_structure 1:1
            }
            bitmap_set(PAGE((u32)mbi), SYSTEM); // mbi structure map 1:1
        }
        else if (mmap->type == MULTIBOOT_MMAP_FREE_MEMORY)
        {
            for (u32 i = 0x0; i < mmap->lenlow; i+= 0x1000)
            {
                if (((mmap->baselow + i) < *kernel_start) || ((mmap->baselow + i) >= *kernel_end))
                    ms_push((u32 *) (mmap->baselow + i));
                else 
                    bitmap_set(PAGE(mmap->baselow + i), SYSTEM); // from kernel_start to kernel_end 
            }   
        }
    }
}

