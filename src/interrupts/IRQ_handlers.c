#include "IRQ_handlers.h"
#include "system.h"
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
	print(TERMINAL, "                         __________\n");
	print(TERMINAL, "                      .~#########\%\%;~.\n");
	print(TERMINAL, "                     /############\%\%;`\\\n");
	print(TERMINAL, "                    /######/~\\/~\\\%\%;,;,\\\n");
	print(TERMINAL, "                   |#######\\    /;;;;.,.|\n");
	print(TERMINAL, "                   |#########\\/\%;;;;;.,.|\n");
	print(TERMINAL, "          XX       |##/~~\\####\%;;;/~~\\;,|       XX\n");
	print(TERMINAL, "        XX..X      |#|  o  \\##\%;/  o  |.|      X..XX\n");
	print(TERMINAL, "      XX.....X     |##\\____/##\%;\\____/.,|     X.....XX\n");
	print(TERMINAL, " XXXXX.....XX      \\#########/\\;;;;;;,, /     XX.....XXXXX\n");
	print(TERMINAL, "X |......XX\%,.@      \\######/\%;\\;;;;, /     @#%,XX......| X\n");
	print(TERMINAL, "X |.....X  @#\%,.@     |######\%\%;;;;,.|     @#\%,.@  X.....| X\n");
	print(TERMINAL, "X  \\...X     @#\%,.@   |# # # \% ; ; ;,|   @#\%,.@     X.../  X\n");
	print(TERMINAL, " X# \\.X        @#\%,.@                  @#\%,.@        X./  #\n");
	print(TERMINAL, "  ##  X          @#\%,.@              @#\%,.@          X   #\n");
	print(TERMINAL, ", \"# #X            @#\%,.@          @#\%,.@            X ##\n");
	print(TERMINAL, "   `###X             @#\%,.@      @#\%,.@             ####'\n");
	print(TERMINAL, "  . ' ###              @#\%.,@  @#\%,.@              ###`\"\n");
	print(TERMINAL, "    . \";\"                @#\%.@#\%,.@                ;\\\"` ' .\n");
	print(TERMINAL, "      '                    @#\%,.@                   ,.\n");
	print(TERMINAL, "      ` ,                @#\%,.@  @@                `\n");
	print(TERMINAL, "                          @@@  @@@\n");
	
	print(TERMINAL, "EXCEPTION: %s", exception_messages[is.int_num]);
	print(TERMINAL, "\n%b", is.err);

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

	__asm__("cli"); // just to make sure not bad gonna happend;D

	calls[irq_number] = caller;

	// masking PIC to enable IRQ handler
	u16 port;
	u8 value;

	if (irq_number < 8) port = PIC1_DATA;
	else port = PIC2_DATA;

	value = in(port) & ~(1 << (irq_number % 8)); 
	out(port, value);	

	__asm__("sti");
}
void irq_remove_call(u8 irq_number)
{
	if (irq_number > 16) return;

	__asm__("cli"); // just to make sure not bad gonna happend;D

	calls[irq_number] = 0;

	// masking PIC to disable IRQ handler
	u16 port;
	u8 value;

	if (irq_number < 8) port = PIC1_DATA;
	else port = PIC2_DATA;

	value = in(port) | (1 << (irq_number % 8)); 
	out(port, value);	
	
	__asm__("sti");

}