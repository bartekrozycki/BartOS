#pragma once
#include "ints.h"
#include "multiboot.h"

#define KERNEL_START 0x100000

typedef struct pm_node {
    u32 base;
    u32 size;
    struct pm_node *next;
}pm_node;

typedef struct pm_queue{
    pm_node *head;
    pm_node *tail;
    u32 iterator;
}pm_queue;


void init_mmu(MultibootInfo* mbi);
void mmu_map(MultibootInfo *mbi);
u8* mmu_malloc(u32 size);
