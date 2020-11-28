#include "pit.h"
#include "interrupts_handlers.h"
#include "print.h"
#include "terminal.h"


void init_pit(void)
{
    // u32 pit_hz = 3579545;// / 3
    irq_new_call(PROGRAMMABLE_INTERRUPT_TIMER_LINE, pit_interrupt);
    print(SERIAL, "[Kernel] PIT Interrupt initalized\n");
}

void pit_interrupt(InterruptSave *is)
{
    *is = *is;
    
}