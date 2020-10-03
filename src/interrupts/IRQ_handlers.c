#include "IRQ_handlers.h"
#include "misc.h"
#include "print.h"

IrqCall calls[16];

const char* exception_messages[32] = {
	"Division by zero",
	"Debug",
	"Non-maskable interrupt",
	"Breakpoint",
	"Detected overflow",
	"Out-of-bounds",
	"Invalid opcode",
	"No coprocessor",
	"Double fault",
	"Coprocessor segment overrun",
	"Bad TSS",
	"Segment not present",
	"Stack fault",
	"General protection fault",
	"Page fault",
	"Unknown interrupt",
	"Coprocessor fault",
	"Alignment check",
	"Machine check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
};

void isr_handler(InterruptSave is)
{   
	print(SERIAL, "%s", exception_messages[is.int_num]);
    __asm__("xchgw %bx, %bx");
    permahalt();
}

void irq_handler(InterruptSave is)
{
	IrqCall caller = calls[is.int_num - 32];

	if (caller)
		caller(&is);

	out(PIC1, PIC_EOI); 
	if (is.int_num >= 40) // PIC2 STARTS FROM GATE 40
		out(PIC2, PIC_EOI);
}

void irq_new_call(u8 irq_number, IrqCall caller)
{
	if (irq_number > 16) return;
	
	calls[irq_number] = caller;
}
void irq_remove_call(u8 irq_number)
{
	if (irq_number > 16) return;

	calls[irq_number] = 0;
}