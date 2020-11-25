#pragma once

#include "ints.h"


#define PAGE_DIRECTORY_SIZE 0x1000
#define PAGE_TABLES_SPACE_SIZE 0x400000

#define KERNEL_BOOT_VMA 0x00100000

#define KERNEL_HIGH_VMA 0xE0000000

#define HEAP_SPACE 0xE8000000
#define HEAP_SIZE  0x08000000

#define DRIVERS_SPACE 0xF0000000

#define KERNEL_STRUCTURES_SPACE 0xF8000000