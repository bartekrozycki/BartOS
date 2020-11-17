#include "boot_memory_init.h"
#include "kernel_panic.h"
#include "bitmap.h"
#include "terminal.h"
#include "print.h"
#include "heap.h"
#include "serial.h"
#include "mStack.h"

extern void Main(MultibootInfo* mboot_info);

void boot_init_mem(u32 mboot_magic, MultibootInfo* mbi, u32 *kernel_start, u32 *kernel_end)
{
	if (mboot_magic != MULTIBOOT_EAX_MAGIC)
		kPanic;
    if (!(mbi->flags & MULTIBOOT_FLAG_MMAP))
        kPanic;
    if ((u32)kernel_end & 0xFFF) kPanic;
    
    u32 kernelStart     = kernel_start;
    u32 kernelEnd       = kernel_end;
    u32 kAllocSpace     = kernel_end;

    PageDirectory   *page_directory;
    PageTableEntry  *page_table_space;
    u8              *bitmap;
    u32             *mem_stack;
    
    u32 highest_address = get_highest_adrress(mbi);
    u32 k_struct_space  = KERNEL_STRUCTURES_SPACE;

    //  p a g i n g    s t r u c t s

    page_directory   = (PageDirectory *)ke_alloc(&kAllocSpace, PAGE_DIRECTORY_SIZE);
    page_table_space = (PageTableEntry *)ke_alloc(&kAllocSpace, PAGE_TABLES_SPACE_SIZE);

    for (u32 i = 0; i < 1024; ++i)
        page_directory[i].entry = ((u8*)page_table_space) + (0x1000 * i);
 
    clearSpace(page_table_space, PAGE_TABLES_SPACE_SIZE);

    // map boot_memory_init :)
    for (u32 i = KERNEL_BOOT_VMA; i < kernel_start; i+= 0x1000)
        map_at(page_directory, i, i);

    for (u32 i = kernel_start; i < kernel_end; i += 0x1000)
        map_at(page_directory, i, i + KERNEL_HIGH_VMA);

    __asm__("xchgw %bx, %bx");

    map_at(page_directory, page_directory, k_struct_space);
    k_struct_space += PAGE_DIRECTORY_SIZE;

    for (u32 i = 0; i < 1024; ++i)
        map_at(page_directory, ((u8*)page_table_space) + (0x1000 * i), 
                               ((u8*)k_struct_space) + (0x1000 * i));
    
    k_struct_space += PAGE_TABLES_SPACE_SIZE;

    //  m e m o r y    b i t m a p    s p a c e

    bitmap = (u8 *) ke_alloc(&kAllocSpace, FRAMES_COUNT(highest_address) / 4);

    clearSpace(bitmap, ALIGN_TO(FRAMES_COUNT(highest_address) / 4, 0x1000));

    for (u32 i = 0, j = ALIGN_TO(FRAMES_COUNT(highest_address) / 4, 0x1000) >> 12; i < j; i++)
        map_at(page_directory, ((u8*)bitmap) + (0x1000 * i), ((u8*)k_struct_space) + (0x1000 * i));
    
    k_struct_space += ALIGN_TO(FRAMES_COUNT(highest_address) / 4, 0x1000);

    // m e m o r y    s t a c k     s p a c e

    mem_stack = (u32 *) ke_alloc(&kAllocSpace, FRAMES_COUNT(highest_address) * 4 + 4);

    for (u32 i = 0, j = (FRAMES_COUNT(highest_address) * 4 + 4) >> 12; i < j; i++)
        map_at(page_directory, ((u8*)mem_stack) + (0x1000 * i), ((u8*)k_struct_space) + (0x1000 * i));

    k_struct_space += ALIGN_TO(FRAMES_COUNT(highest_address) * 4  + 4, 0x1000);

    __asm__("xchgw %bx, %bx");
    enablePaging(page_directory);

    kPanic;
}

void * ke_alloc(u32* space, u32 size)
{
    u32 addr = *space;
    size = ALIGN_TO(size, 0x1000);

    *space += size;

    return (void *) addr;
}

void map_at(PageDirectory * page_directory, void * physaddr, void * virtualaddr)
{
    if (((u32)physaddr & 0xFFF) || ((u32)virtualaddr & 0xFFF))
        kPanic;

    u32 pdindex = (u32)virtualaddr >> 22;
    u32 ptindex = (u32)virtualaddr >> 12 & 0x03FF;
    
    if (!((PageDirectory *)page_directory)[pdindex].present)
        ((PageDirectory *)page_directory)[pdindex].present = 1;

    PageTableEntry *PTE = (PageTableEntry *) (((PageDirectory *)page_directory)[pdindex].address << 12);

    PTE[ptindex].entry = ((u32)physaddr) | 0x01; // Present
}

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

