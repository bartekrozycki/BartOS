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

typedef void (*IrqCall)(InterruptSave *is); // function pointer to interrupts handler

void isr_handler(InterruptSave is);
void irq_handler(InterruptSave is);

void irq_new_call(u8 irq_number, IrqCall caller);
void irq_remove_call(u8 irq_number);