#include "ints.h"
#include "misc.h"
#include "terminal.h"
#include "idt.h"
#include "lib.h"
#include "lib.h"

struct _IDT idt[256];
struct _IDT_Descriptor idt_ptr;

extern void load_idt(u32 idt_ptr);

extern void irq0();extern void irq1();
extern void irq2();extern void irq3();
extern void irq4();extern void irq5();
extern void irq6();extern void irq7();
extern void irq8();extern void irq9();
extern void irq10();extern void irq11();
extern void irq12();extern void irq13();
extern void irq14();extern void irq15();

extern void isr0(); extern void isr1(); extern void isr2();extern void isr3(); 
extern void isr4(); extern void isr5();extern void isr6();extern void isr7(); 
extern void isr8();extern void isr9();extern void isr10();extern void isr11();
extern void isr12();extern void isr13(); extern void isr14();extern void isr15();
extern void isr16();extern void isr17();extern void isr18();extern void isr19();
extern void isr20();extern void isr21();extern void isr22();extern void isr23();
extern void isr24();extern void isr25();extern void isr26();extern void isr27();
extern void isr28();extern void isr29();extern void isr30();extern void isr31();

extern void isr186();

void idt_set_gate(u8 n, u32 base, u16 selector, u8 flags);

void idt_init(void)
{

	ASSERT(struct _IDT, 8)
	ASSERT(struct _IDT_Descriptor, 6)

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

	idt_set_gate(0,  (u32) isr0,  0x08, 0x8E);
	idt_set_gate(1,  (u32) isr1,  0x08, 0x8E);
	idt_set_gate(2,  (u32) isr2,  0x08, 0x8E);
	idt_set_gate(3,  (u32) isr3,  0x08, 0x8E);
	idt_set_gate(4,  (u32) isr4,  0x08, 0x8E);
	idt_set_gate(5,  (u32) isr5,  0x08, 0x8E);
	idt_set_gate(6,  (u32) isr6,  0x08, 0x8E);
	idt_set_gate(7,  (u32) isr7,  0x08, 0x8E);
	idt_set_gate(8,  (u32) isr8,  0x08, 0x8E);
	idt_set_gate(9,  (u32) isr9,  0x08, 0x8E);
	idt_set_gate(10, (u32) isr10, 0x08, 0x8E);
	idt_set_gate(11, (u32) isr11, 0x08, 0x8E);
	idt_set_gate(12, (u32) isr12, 0x08, 0x8E);
	idt_set_gate(13, (u32) isr13, 0x08, 0x8E);
	idt_set_gate(14, (u32) isr14, 0x08, 0x8E);
	idt_set_gate(15, (u32) isr15, 0x08, 0x8E);
	idt_set_gate(16, (u32) isr16, 0x08, 0x8E);
	idt_set_gate(17, (u32) isr17, 0x08, 0x8E);
	idt_set_gate(18, (u32) isr18, 0x08, 0x8E);
	idt_set_gate(19, (u32) isr19, 0x08, 0x8E);
	idt_set_gate(20, (u32) isr20, 0x08, 0x8E);
	idt_set_gate(21, (u32) isr21, 0x08, 0x8E);
	idt_set_gate(22, (u32) isr22, 0x08, 0x8E);
	idt_set_gate(23, (u32) isr23, 0x08, 0x8E);
	idt_set_gate(24, (u32) isr24, 0x08, 0x8E);
	idt_set_gate(25, (u32) isr25, 0x08, 0x8E);
	idt_set_gate(26, (u32) isr26, 0x08, 0x8E);
	idt_set_gate(27, (u32) isr27, 0x08, 0x8E);
	idt_set_gate(28, (u32) isr28, 0x08, 0x8E);
	idt_set_gate(29, (u32) isr29, 0x08, 0x8E);
	idt_set_gate(30, (u32) isr30, 0x08, 0x8E);
	idt_set_gate(31, (u32) isr31, 0x08, 0x8E);

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

	idt_set_gate(0xBA, (u32) isr186, 0x08, 0x8E);


	load_idt((u32) &idt_ptr);


	terminal_writestring("[Kernel] IDT Initialized");
}

void idt_set_gate(u8 n, u32 base, u16 selector, u8 flags) {
	idt[n].base_low  = (base & 0x0000FFFF);
	idt[n].selector  = selector;
	idt[n].zero      = 0;
	idt[n].flags	 = flags ;//| 0b1100000;
	// idt[n].type_attr.flags = flags;
	// idt[n].type_attr.present = 1;
	// idt[n].type_attr.descriptor_privilege_level = 2;
	
	idt[n].base_high = (base & 0xFFFF0000) >> 16;
}