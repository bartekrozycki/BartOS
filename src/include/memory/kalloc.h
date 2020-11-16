#pragma once
#include "bitmap.h"
#include "system.h"
#include "boot_memory_init.h"

void *kalloc();
void kfree(void *x);

