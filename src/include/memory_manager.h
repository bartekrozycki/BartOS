#pragma once

#include "ints.h"
#include "multiboot.h"
#include "paging.h"


extern u8 *_kernel_end;

typedef struct memory_stack
{
    u32 *address;
    memory_stack *next;
}memory_stack;

typedef struct MManager
{
    struct {
        u8 *address;
        u32 size_in_bytes;
    } Bitmap;

    struct {
        memory_stack *address;
        u32 size_in_bytes;
    } Stack;

    struct {
        PageDirectory * PD_address;
    } Pagging;

    struct {
        u32 KernelBootAddress;
        u32 KernelEndAddress;
        u32 KernelHighAddress;
        u32 HighestAddress;
    } Memory;
} MManager;

u32 init_memory_manager(MultibootInfo* mbi);

u32 get_highest_adrress(MultibootInfo* mbi);
void init_bitmap_space();
void init_stack_space();

void enablePaging(PageDirectory* pd_addr);
