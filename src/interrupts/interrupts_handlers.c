#include "interrupts_handlers.h"
#include "io_bus.h"
#include "print.h"
#include "kernel_panic.h"

IrqCall IrqCalls[16] = {0};
IsrCall IsrCalls[32] = {0};

const char *exception_messages[32] = {
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
//https://www.asciiart.eu/
void isr_handler(InterruptSave is)
{
	if (IsrCalls[is.int_num] != 0)
		IsrCalls[is.int_num](&is);
	else {
		print(SERIAL, "                         __________\n");
		print(SERIAL, "                      .~#########\%\%;~.\n");
		print(SERIAL, "                     /############\%\%;`\\\n");
		print(SERIAL, "                    /######/~\\/~\\\%\%;,;,\\\n");
		print(SERIAL, "                   |#######\\    /;;;;.,.|\n");
		print(SERIAL, "                   |#########\\/\%;;;;;.,.|\n");
		print(SERIAL, "          XX       |##/~~\\####\%;;;/~~\\;,|       XX\n");
		print(SERIAL, "        XX..X      |#|  o  \\##\%;/  o  |.|      X..XX\n");
		print(SERIAL, "      XX.....X     |##\\____/##\%;\\____/.,|     X.....XX\n");
		print(SERIAL, " XXXXX.....XX      \\#########/\\;;;;;;,, /     XX.....XXXXX\n");
		print(SERIAL, "X |......XX\%,.@      \\######/\%;\\;;;;, /     @#%,XX......| X\n");
		print(SERIAL, "X |.....X  @#\%,.@     |######\%\%;;;;,.|     @#\%,.@  X.....| X\n");
		print(SERIAL, "X  \\...X     @#\%,.@   |# # # \% ; ; ;,|   @#\%,.@     X.../  X\n");
		print(SERIAL, " X# \\.X        @#\%,.@                  @#\%,.@        X./  #\n");
		print(SERIAL, "  ##  X          @#\%,.@              @#\%,.@          X   #\n");
		print(SERIAL, ", \"# #X            @#\%,.@          @#\%,.@            X ##\n");
		print(SERIAL, "   `###X             @#\%,.@      @#\%,.@             ####'\n");
		print(SERIAL, "  . ' ###              @#\%.,@  @#\%,.@              ###`\"\n");
		print(SERIAL, "    . \";\"                @#\%.@#\%,.@                ;\\\"` ' .\n");
		print(SERIAL, "      '                    @#\%,.@                   ,.\n");
		print(SERIAL, "      ` ,                @#\%,.@  @@                `\n");
		print(SERIAL, "                          @@@  @@@\n");

        print(SERIAL, "\n---====[ Exception.\n");
        print(SERIAL, "Error code: %b\nError Message: %s\n", is.err, exception_messages[is.int_num]);
        print(SERIAL,
              "Info EAX %p\nInfo EBX %p\nInfo ECX %p\nInfo EDX %p\nInfo ESP %p\nInfo EBP %p\nInfo ESI %p\nInfo EDI %p\n",
              is.eax, is.ebx, is.ecx, is.edx, is.esp, is.ebp, is.esi, is.edi);
        print(SERIAL, "Info EIP %p\nInfo CS %p\nInfo EFLAGS %p\nInfo USER_ESP %p\nInfo SS %p\n",
              is.eip, is.cs, is.eflags, is.useresp, is.ss);
		print(SERIAL, "Int Num %d", is.int_num);
		print(SERIAL, "]====---\n");

		kPanic;
	}
}

void irq_handler(InterruptSave is)
{
	IrqCall caller = IrqCalls[is.int_num - 32];

	if (caller)
		caller(&is);

	out(PIC1, PIC_EOI);
	if (is.int_num >= 40) // PIC2 STARTS FROM GATE 40
		out(PIC2, PIC_EOI);
}

void irq_new_call(u8 irq_number, IrqCall caller)
{
	if (irq_number > 16)
		return;

	__asm__("cli"); // just to make sure not bad gonna happend;D

	IrqCalls[irq_number] = caller;

	// masking PIC to enable IRQ handler
	u16 port;
	u8 value;

	if (irq_number < 8)
		port = PIC1_DATA;
	else
		port = PIC2_DATA;

	value = in(port) & ~(1 << (irq_number % 8));
	out(port, value);

	__asm__("sti");
}
void isr_new_call(u8 isr_number, IsrCall caller)
{
	if (isr_number > 32)
		return;
	else if (IsrCalls[isr_number] != 0) 
		kPanic;

	IsrCalls[isr_number] = caller;
}
void irq_remove_call(u8 irq_number)
{
	if (irq_number > 16)
		return;

	__asm__("cli"); // just to make sure not bad gonna happend;D

	IrqCalls[irq_number] = 0;

	// masking PIC to disable IRQ handler
	u16 port;
	u8 value;

	if (irq_number < 8)
		port = PIC1_DATA;
	else
		port = PIC2_DATA;

	value = in(port) | (1 << (irq_number % 8));
	out(port, value);

	__asm__("sti");
}
void isr_remove_call(u8 isr_number)
{
	if (isr_number > 32)
		return;
	IsrCalls[isr_number] = 0;
}