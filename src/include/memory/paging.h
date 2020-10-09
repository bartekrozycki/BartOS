#pragma once

#include "ints.h"

typedef union PageDirectoryEntry {
    u32 entry; // 4KiB aligned so 0x1000 - 12bits always 0
    struct {
        u8 present:1; //1 bit 'obecności'
        u8 rw:1; // Read/Write
        u8 us:1; // User/Supervisor
        u8 pwt:1; //0 Page-level write-through
        u8 pcd:1; //0 Page-level cache disable
        u8 acc:1; // Accessed 
        u8 _ignored:1;
        u8 ps:1; // If CR4.PSE = 1, must be 0; otherwise, ignored
        u8 free_space:4;
        u32 address:20;
    }__attribute__((packed));
}PageDirectoryEntry;

typedef union PageTableEntry {
    u32 entry; // 4KiB aligned so 0x1000 - 12bits always 0
    struct {
        u8 present:1; //1 bit 'obecności'
        u8 rw:1; // Read/Write
        u8 us:1; // User/Supervisor
        u8 pwt:1; //0 Page-level write-through
        u8 pcd:1; //0 Page-level cache disable
        u8 acc:1; // Accessed 
        u8 dirt:1;
        u8 pat:1; // If the PAT is supported,
        u8 g:1; // Global; if CR4.PGE = 1
        u8 free_space:3;
        u32 address:20;
    }__attribute__((packed));
} PageTableEntry;