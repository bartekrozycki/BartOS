#pragma once
#include "physical_memory_bitmap.h"
#include "boot_memory_init.h"

void init_kalloc(MultibootInfo *mbi, u32 kernel_start, u32 kernel_end);
void *kalloc();
void kfree(void *x);

