#include "pit.h"
#include "interrupts_handlers.h"
#include "print.h"
#include "io_bus.h"

#define PIT_HZ              1193182
#define PIT_DIVIDER_MAX     65536

u16 pit_div_find_best(u32 hz) {
    u16 div;

    for (div = 1; div < (PIT_DIVIDER_MAX - 1); div++)
        if ((u32) (PIT_HZ / div) < hz)
            break;
    if (div%2 != 0 && ((PIT_HZ / div) - hz) > (hz - (PIT_HZ / div - 1)))
        --div;
    return div;
}

static u32 timer_tick = 0;
static u32 hertz = 0;

void init_pit(u32 hz) {

    u16 div = pit_div_find_best(hz);

    out(PIT_COMMAND, PIT_SETTING);
    out(PIT_CHANNEL_0, div & 0xFF);
    out(PIT_CHANNEL_0, div >> 8);
    hertz = hz;

    irq_new_call(PROGRAMMABLE_INTERRUPT_TIMER_LINE, pit_interrupt);
    print(SERIAL, "[Kernel] PIT Interrupt initalized with div %d => %d Hz\n", div, hertz);
}


void pit_interrupt(InterruptSave *is) {
    ++timer_tick;
}