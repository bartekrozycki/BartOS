#pragma once

#include "ints.h"

extern u32 _kernel_start;
extern u32 _kernel_end;

#define KERNEL_BOOT_VMA 0x00100000

#define KERNEL_HIGH_VMA 0xE0000000
#define HEAP_SPACE 0xE8000000
#define DRIVERS_SPACE 0xF0000000
#define KERNEL_STRUCTURES_SPACE 0xF8000000