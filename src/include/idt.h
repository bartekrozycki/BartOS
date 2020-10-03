#pragma once

struct _IDT{
	u16 base_low;
	u16 selector;
	u8  zero;
	u8 flags;
	u16 base_high;
} __attribute__((packed));

struct _IDT_Descriptor{
	u16 limit;
	u32 base;
} __attribute__((packed));

void idt_init(void);