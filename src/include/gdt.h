#pragma once

struct _GDT{
	u16 limit_low;
	u16 base_low;
	u8  base_middle;
	u8  access;
	u8  limit_high_and_flags;
	u8  base_high;
}__attribute((packed));

struct _GDT_Descriptor{
	u16 limit;
	u32 base;
}__attribute((packed));

void init_gdt(void);
