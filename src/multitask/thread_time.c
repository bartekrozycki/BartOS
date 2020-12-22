//
// Created by bartosz on 16.12.2020.
//
#include "threads.h"
#include "print.h"
#include "pit.h"
#include "kernel_panic.h"

#define MILI 1000

volatile i32 timer_tick = 0;
volatile i32 last_read_tick = 0;

void pit_interrupt(InterruptSave *is) {
    lock_postpone();
    {
        ++timer_tick;

        if (threads_sleeping->size > 0) // something sleeping
        {
            struct thread_control_block_t *ptr = threads_sleeping->head;

            while (ptr)
            {
                if (ptr->paused_until <= timer_tick)
                {
                    list_thread_remove(threads_sleeping, ptr);
                    unblock_task(ptr);
                }

                ptr = ptr->next;
            }
        }
        if (current_running_tcb)
            if (timer_tick % 3 == 0)
                schedule();
    }
    unlock_postpone();
}

void update_time_used(void)
{
    i32 c = last_read_tick;
    last_read_tick = timer_tick;

    if (current_running_tcb == (thread_control_block *)0)
        idleCPUTime += (last_read_tick - c);
    else
        current_running_tcb->time_used += (last_read_tick - c);
}

void mili_sleep_until(i32 until)
{
    lock_postpone();

    if(until < timer_tick) {
        unlock_postpone();
        return;
    }

    current_running_tcb->paused_until = until;
    list_thread_push_back(threads_sleeping, current_running_tcb);


    unlock_postpone();

    block_task(THREAD_SLEEPING);
    return;
}
void mili_sleep(i32 mili_seconds)
{
    mili_sleep_until(timer_tick + mili_seconds);
}
void sleep(i32 seconds)
{
    __asm__("cli");
    mili_sleep(seconds * MILI);
}