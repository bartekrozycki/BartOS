#include "boot_memory_init.h"
#include "system.h"
#include "bitmap.h"
#include "terminal.h"
#include "print.h"


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
    u32 memory_stack_size   = FRAMES_COUNT(highest_address) * 4 + 4; // [AAAAxxxxxxxxxxxxxxxxxxxx] (u32) stackpointer

    u32* bitmap     = (u32 *) ke_alloc(&kernel_end, memory_bitmap_size);
    u32* stack      = (u32 *) ke_alloc(&kernel_end, memory_stack_size);
    u32* page_dir   = (u32 *) ke_alloc(&kernel_end, sizeof(PageDirectory) * 0x400);

    initial_kernel_paging(&kernel_end, (PageDirectory* ) page_dir, mbi); 

    init_stack(stack); 
    init_bitmap(bitmap);
    init_memory(mbi, &kernel_start, &kernel_end);

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

    for (u8 *i = (u8 *)*kernel_end, 
            *j = (u8 *)(*kernel_end + size); i < j; i++) *i = 0;  

    *kernel_end += size;

    if (*kernel_end & 0xFFF) permahalt();
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
 * @param ke Kernel end adderss
 * @param pd Page directory
 */
void initial_kernel_paging(u32 * ke, PageDirectory *pd, MultibootInfo *mbi)
{
    for (MultibootMemoryMap *mmap = (MultibootMemoryMap *) mbi->mmap_address;
        (u32) mmap < (mbi->mmap_address + mbi->mmap_length); 
        mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size)))
    {
        if (mmap->type == MULTIBOOT_MMAP_RESERVED && mmap->baselow < KERNEL_BOOT_VMA)
        {
            for (u32 i = 0x0; i < mmap->lenlow; i+= 0x1000)
            {
                map(ke, pd, mmap->baselow + i, mmap->baselow + i);
            }
        }
    }
    map(ke, pd, (u32) mbi, (u32) mbi);

    for (u32 i = KERNEL_BOOT_VMA; i < (u32)*ke; i += 0x1000)
    {
        map(ke, pd, i, i);
        map(ke, pd, i, 0xE0000000 + i);
    }
    map(ke, pd, 0xB8000, 0xF0001000); // wideło

    enablePaging(pd);
}
/**
 * Do not use later :)
 * @param ke Kernel end address
 * @param pd PageDirectory address
 * @param physaddr Physical address
 * @param virtualaddr Virtual address
 * @return permahalt() if PTE not exist or virtual address already mapped
 */
void map(u32 *ke, PageDirectory * pd, u32 physaddr, u32 virtualaddr)
{
   // Make sure that both addresses are page-aligned.
    if (((u32)physaddr & 0xFFF) || ((u32)virtualaddr & 0xFFF))
        permahalt();

    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
    
    if (!((PageDirectory *)pd)[pdindex].present)
    {
        u32 new_page_entry = ((u32)ke_alloc((u32 *)ke, 0x1000));
        ((PageDirectory *)pd)[pdindex].entry = new_page_entry | 0x1;
        map(ke, pd, new_page_entry, new_page_entry);
    }
    PageTableEntry *pt = (PageTableEntry *) (((PageDirectory *)pd)[pdindex].address << 12);

    pt[ptindex].entry = ((unsigned long)physaddr) | 0x01; // Present
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

