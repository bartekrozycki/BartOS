#pragma once
#include "ints.h"
#include "multiboot.h"
#include "paging.h"
#include "mStack.h"

extern u8 *_kernel_end;

typedef struct MManager
{
    struct {
        u8 *address;
        u32 size_in_bytes;
    } Bitmap;

    struct {
        memory_stack *address;
        memory_stack *ptr;
        u32 free;
        u32 size_in_bytes;
    } Stack;

    struct {
        PageDirectory * PD_address;
    } Pagging;

    struct {
        u32 KernelEndAddress;
        u32 HighestAddress;
    } Memory;
} MManager;

u32 init_memory_manager(MultibootInfo* mbi);
u32 get_highest_adrress(MultibootInfo* mbi);
u8* ke_alloc(u32 size);

void initial_kernel_paging();
void enablePaging(PageDirectory* pd_addr);

void init_physical(MultibootInfo* mbi);
