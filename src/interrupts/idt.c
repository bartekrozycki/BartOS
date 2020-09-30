#include "ints.h"
#include "misc.h"
#include "terminal.h"
#include "idt.h"
#include "lib.h"

struct {
	u16 base_low;
	u16 selector;
	u8  zero;
	u8 flags;
	u16 base_high;
} __attribute__((packed)) idt[256];

struct {
	u16 limit;
	u32 base;
} __attribute__((packed)) idt_ptr;


extern void load_idt(u32 idt_ptr);
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void idt_set_gate(u8 n, u32 base, u16 selector, u8 flags);

void idt_init(void)
{
	idt_ptr.limit = sizeof(idt) - 1;
	idt_ptr.base  = (u32) &idt;

	memset(idt, 0, sizeof(idt));


	out(0x20, 0x11);
	out(0xA0, 0x11);
	out(0x21, 0x20);
	out(0xA1, 0x28); //0x28
	out(0x21, 0x04);
	out(0xA1, 0x02);
	out(0x21, 0x01);
	out(0xA1, 0x01);
	out(0x21, 0x00);
	out(0xA1, 0x00);

	// IRQs
	idt_set_gate(32, (u32) irq0,  0x08, 0x8E);
	idt_set_gate(33, (u32) irq1,  0x08, 0x8E);
	idt_set_gate(34, (u32) irq2,  0x08, 0x8E);
	idt_set_gate(35, (u32) irq3,  0x08, 0x8E);
	idt_set_gate(36, (u32) irq4,  0x08, 0x8E);
	idt_set_gate(37, (u32) irq5,  0x08, 0x8E);
	idt_set_gate(38, (u32) irq6,  0x08, 0x8E);
	idt_set_gate(39, (u32) irq7,  0x08, 0x8E);
	idt_set_gate(40, (u32) irq8,  0x08, 0x8E);
	idt_set_gate(41, (u32) irq9,  0x08, 0x8E);
	idt_set_gate(42, (u32) irq10, 0x08, 0x8E);
	idt_set_gate(43, (u32) irq11, 0x08, 0x8E);
	idt_set_gate(44, (u32) irq12, 0x08, 0x8E);
	idt_set_gate(45, (u32) irq13, 0x08, 0x8E);
	idt_set_gate(46, (u32) irq14, 0x08, 0x8E);
	idt_set_gate(47, (u32) irq15, 0x08, 0x8E);

	load_idt((u32) &idt_ptr);

	terminal_writestring("[KERNEL] IDT Initialized");
	
}

void idt_set_gate(u8 n, u32 base, u16 selector, u8 flags) {
	idt[n].base_low  = (u16) (base & 0x0000FFFF);
	idt[n].selector  = selector;
	idt[n].zero      = 0;
	idt[n].flags	 = flags ;//| 0b1100000;
	// idt[n].type_attr.flags = flags;
	// idt[n].type_attr.present = 1;
	// idt[n].type_attr.descriptor_privilege_level = 2;
	
	idt[n].base_high = (u16) (base & 0xFFFF0000) >> 16;
}