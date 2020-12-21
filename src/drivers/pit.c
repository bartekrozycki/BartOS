#include "pit.h"
#include "interrupts_handlers.h"
#include "print.h"
#include "io_bus.h"
#include "kernel_panic.h"
#include "threads.h"
#include "thread_list.h"

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
static u32 last_read_tick = 0;
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
    lock_stuff();
    if (threads_sleeping->size > 0) // something sleeping
    {
        struct thread_control_block_t *ptr = threads_sleeping->head;

        while (ptr)
        {
            if (ptr->paused_until <= timer_tick)
            {
                ptr->state = THREAD_RUNNING;
                unblock_task(ptr);
            }
            ptr = ptr->next;
        }
    }
    unlock_stuff();
}
u32 read_counter (void)
{
    return last_read_tick = timer_tick;
}
