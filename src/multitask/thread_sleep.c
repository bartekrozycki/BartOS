//
// Created by bartosz on 16.12.2020.
//
#include "threads.h"
#include "print.h"
#include "pit.h"
#include "kernel_panic.h"

#define MILI 1000

void mili_sleep_until(u32 until)
{
    lock_stuff();
    if(until < read_counter()) {
        unlock_stuff();
        return;
    }
    running_thread->paused_until = until;

    unlock_stuff();

    block_task(THREAD_PAUSED);
    return;
}
void mili_sleep(u32 delay)
{
    mili_sleep_until(read_counter() + delay);
}
void sleep(u32 seconds)
{
    __asm__("cli");
    mili_sleep(seconds * MILI);
}