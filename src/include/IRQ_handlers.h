#pragma once

#include "ints.h"

typedef struct InterruptSave{
	// segment registers
	u32 gs;
	u32 fs;
	u32 es;
	u32 ds;
	// pusha registers
	u32 edi;
	u32 esi;
	u32 ebp;
	u32 esp;
	u32 ebx;
	u32 edx;
	u32 ecx;
	u32 eax;
	// interrupt info
	u32 int_num;
	u32 err;
	// pushed automatically
	u32 eip;
	u32 cs;
	u32 eflags;
	u32 useresp;
	u32 ss;
} __attribute__((packed)) InterruptSave;

void isr_handler(InterruptSave is);

void irq0_handler(void);
void irq1_handler(void);
void irq2_handler(void);
void irq3_handler(void);
void irq4_handler(void);
void irq5_handler(void);
void irq6_handler(void);
void irq7_handler(void);
void irq8_handler(void);
void irq9_handler(void);
void irq10_handler(void);
void irq11_handler(void);
void irq12_handler(void);
void irq13_handler(void);
void irq14_handler(void);
void irq15_handler(void);