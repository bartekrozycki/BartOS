#include "boot_memory_init.h"
#include "kernel_panic.h"
#include "physical_memory_bitmap.h"
#include "terminal.h"
#include "print.h"
#include "serial.h"
#include "physical_memory_stack.h"
#include "kernel_frame_alloc.h"

extern void Main(MultibootInfo *mboot_info);

void boot_init_mem(u32 mboot_magic, MultibootInfo *mbi, u32 *kernel_start, u32 *kernel_end) {
    if (mboot_magic != MULTIBOOT_EAX_MAGIC)
        kPanic;
    if (!(mbi->flags & MULTIBOOT_FLAG_MMAP))
        kPanic;
    if ((u32) kernel_end & 0xFFF)
        kPanic;

    u32 kernelStart = (u32) kernel_start;
    u32 kernelEnd = (u32) kernel_end;
    u32 kAllocSpace = (u32) kernel_end;

    PageDirectory *page_directory;
    PageTableEntry *page_table_space;
    u8 *mem_bitmap_ptr;
    u8 *mem_stack_ptr;

    u32 highest_address = get_highest_adrress(mbi);
    u32 k_struct_space = KERNEL_STRUCTURES_SPACE;
    u32 mem_stack_bytes = FRAMES_COUNT(highest_address) * 4 + 4;
    u32 mem_bitmap_bytes = FRAMES_COUNT(highest_address) / 4;

    //  p a g i n g    s t r u c t s

    page_directory = (PageDirectory *) ke_alloc(&kAllocSpace, PAGE_DIRECTORY_SIZE);
    page_table_space = (PageTableEntry *) ke_alloc(&kAllocSpace, PAGE_TABLES_SPACE_SIZE);

    for (u32 i = 0; i < 1024; ++i)
        page_directory[i].entry = (u32) ((u8 *) page_table_space) + (0x1000 * i);

    clearSpace(page_table_space, PAGE_TABLES_SPACE_SIZE);

    for (u32 i = KERNEL_BOOT_VMA; i < kAllocSpace; i += 0x1000)
        map_at(page_directory, (u32 *) i, (u32 *) i);

    // k e r n e l   m a p    h i g h
    for (u32 i = kernelStart; i < kernelEnd; i += 0x1000)
        map_at(page_directory, (u32 *) i, (u32 *) (i + KERNEL_HIGH_VMA));

    // p a g i n g    s t r u c t s    m a p
    map_at(page_directory, (u32 *) page_directory, (u32 *) k_struct_space);
    k_struct_space += PAGE_DIRECTORY_SIZE;

    for (u32 i = 0; i < 1024; ++i)
        map_at(page_directory, ((u8 *) page_table_space) + (0x1000 * i),
               ((u8 *) k_struct_space) + (0x1000 * i));

    k_struct_space += PAGE_TABLES_SPACE_SIZE;



    //  m e m o r y    b i t m a p    s p a c e

    mem_bitmap_ptr = (u8 *) ke_alloc(&kAllocSpace, mem_bitmap_bytes);
    clearSpace(mem_bitmap_ptr, ALIGN_TO(mem_bitmap_bytes, 0x1000));

    for (u32 i = 0, j = ALIGN_TO(mem_bitmap_bytes, 0x1000) >> 12; i < j; i++)
        map_at(page_directory, mem_bitmap_ptr + (0x1000 * i), ((u8 *) k_struct_space) + (0x1000 * i));

    mem_bitmap_ptr = (u8 *) k_struct_space;
    k_struct_space += ALIGN_TO(mem_bitmap_bytes, 0x1000);

    // m e m o r y    s t a c k     s p a c e

    mem_stack_ptr = (u8 *) ke_alloc(&kAllocSpace, mem_stack_bytes);

    for (u32 i = 0, j = mem_stack_bytes >> 12; i < j; i++)
        map_at(page_directory, mem_stack_ptr + (0x1000 * i), ((u8 *) k_struct_space) + (0x1000 * i));

    mem_stack_ptr = (u8 *) k_struct_space;
    k_struct_space += ALIGN_TO(mem_stack_bytes, 0x1000); //// <---------------- kernel allocated memory end

    // M u l t i    b o o t    i n f o r m a t i o n
    map_at(page_directory, mbi, mbi);
    for (MultibootMemoryMap *mmap = (MultibootMemoryMap *) mbi->mmap_address;
         (u32) mmap < (mbi->mmap_address + mbi->mmap_length);
         mmap = (MultibootMemoryMap *) ((u32) mmap + mmap->size + sizeof(mmap->size))) {
        if (mmap->type == MULTIBOOT_MMAP_RESERVED && mmap->baselow < KERNEL_BOOT_VMA)
            for (u32 i = 0x0; i < mmap->lenlow; i += 0x1000)
                map_at(page_directory, (u32 *) (mmap->baselow + i), (u32 *) (mmap->baselow + i));
    }

    map_at(page_directory, (u32 *) 0xb8000, (u32 *) 0xF0000000);

    enablePaging(page_directory);
    init_serial();


    setBitmapAddress((u32 *) mem_bitmap_ptr);

    setMStackAdress((u32 *) mem_stack_ptr);

    init_kalloc(mbi, (u32) KERNEL_BOOT_VMA, kAllocSpace);

#ifdef DEBUG
    print(SERIAL, "warning Kernel space from %p to %p\n", KERNEL_BOOT_VMA, kAllocSpace);
    print(SERIAL, "warning Page Directory: %p\n", page_directory);
    print(SERIAL, "warning Page Table Entrys: %p\n", page_table_space);
    print(SERIAL, "warning Memory Bitmap: %p size %p\n", mem_bitmap_ptr, ALIGN_TO(mem_bitmap_bytes, 0x1000));
    print(SERIAL, "warning Memory Stack %p size %p\n\n", mem_stack_ptr, ALIGN_TO(mem_stack_bytes, 0x1000));
#endif

    Main(mbi);

    kPanic;
}

void *ke_alloc(u32 *space, u32 size) {
    u32 addr = *space;
    size = ALIGN_TO(size, 0x1000);

    (*space) += size;

    return (void *) addr;
}

// can be used only in early stage :)
void map_at(PageDirectory *page_directory, void *physaddr, void *virtualaddr) {
    if (((u32) physaddr & 0xFFF) || ((u32) virtualaddr & 0xFFF)) {
        physaddr = (void *) (((u32) physaddr) & ~(((u32) physaddr) & 0xFFF));
        virtualaddr = (void *) (((u32) virtualaddr) & ~(((u32) virtualaddr) & 0xFFF));
    }

    u32 pd_index = (u32) virtualaddr >> 22;
    u32 pt_index = (u32) virtualaddr >> 12 & 0x03FF;

    if (!((PageDirectory *) page_directory)[pd_index].present)
        ((PageDirectory *) page_directory)[pd_index].present = 1;

    PageTableEntry *PTE = (PageTableEntry *) (((PageDirectory *) page_directory)[pd_index].address << 12);

    PTE[pt_index].entry = ((u32) physaddr) | 0x01; // Present
}