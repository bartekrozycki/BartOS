#include "IRQ_handlers.h"
#include "misc.h"
#include "terminal.h"
#include "print.h"

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

    printf("Exception: %s", exception_messages[is.int_num]);

    __asm__("xchgw %dx, %dx");

    permahalt();
}

void irq0_handler(void) {
    out(0x20, 0x20); //EOI
}

void irq1_handler(void) {
    if (in(0x64) & 0x01)
    {   
        u8 keycode = in(0x60);

        terminal_putchar((char) keycode);
    }
    out(0x20, 0x20); //EOI
}

void irq2_handler(void) {
    out(0x20, 0x20); //EOI
}

void irq3_handler(void) {
    out(0x20, 0x20); //EOI
}

void irq4_handler(void) {
    out(0x20, 0x20); //EOI
}

void irq5_handler(void) {
    out(0x20, 0x20); //EOI
}

void irq6_handler(void) {
    out(0x20, 0x20); //EOI
}

void irq7_handler(void) {
    out(0x20, 0x20); //EOI
}

void irq8_handler(void) {
    out(0xA0, 0x20);
    out(0x20, 0x20); //EOI          
}

void irq9_handler(void) {
    out(0xA0, 0x20);
    out(0x20, 0x20); //EOI
}

void irq10_handler(void) {
    out(0xA0, 0x20);
    out(0x20, 0x20); //EOI
}

void irq11_handler(void) {
    out(0xA0, 0x20);
    out(0x20, 0x20); //EOI
}

void irq12_handler(void) {
    out(0xA0, 0x20);
    out(0x20, 0x20); //EOI
}

void irq13_handler(void) {
    out(0xA0, 0x20);
    out(0x20, 0x20); //EOI
}

void irq14_handler(void) {
    out(0xA0, 0x20);
    out(0x20, 0x20); //EOI
}

void irq15_handler(void) {
    out(0xA0, 0x20);
    out(0x20, 0x20); //EOI
}
