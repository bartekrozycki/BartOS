#pragma once
#include "bitmap.h"
#include "boot_memory_init.h"

void *kalloc();
void kfree(void *x);

