#include "threads.h"
#include "print.h"
#include "kernel_panic.h"

static volatile i32 IRQ_disable_counter = 0;
volatile i32 postpone_task_switches_counter = 0;
volatile i32 task_switches_postponed_flag = 0;

void lock_scheduler(void)
{
    __asm__("cli");
    ++IRQ_disable_counter;
}
void unlock_scheduler(void)
{
    --IRQ_disable_counter;
    if (IRQ_disable_counter == 0)
            __asm__("sti");
}

/**
 * new thread
 * lock ~ cli
 * schedule
 * .........task switch ~ other task
 * .........task_entry -> unlock
 * .........Interrupt -> lock
 * unlock
 * ......................................
 * [PIT Interrupt]
 * ~ cli
 * ~ send EOI
 * ~ handle interrupt
 * lock
 * schedule
 * task switch
 * unlock -----------> if task has been switched again,
 *                     continuation of this will start here
 *                     If task is new it has thread_entry that unlock scheduler
**/
volatile i32 idleCPUTime = 0;

void schedule(void)
{
    if(postpone_task_switches_counter != 0) {
        task_switches_postponed_flag = 1;
        return;
    }

    thread_control_block *next = list_thread_pop_front(threads_ready);

    if (next != NULL)
    {
        if (current_running_tcb->state == THREAD_RUNNING) // if thread has other status than RUNNING dont push back it to queue of running task
            list_thread_push_back(threads_ready, current_running_tcb);

        switch_to_thread(&current_running_tcb, next);
    }
    else if (current_running_tcb->state == THREAD_RUNNING)
        return;
    else
    {
        thread_control_block *task = current_running_tcb;
        current_running_tcb = (thread_control_block *)0;

        do {
            __asm__("sti");
            __asm__("hlt");
            __asm__("cli");
            next = list_thread_pop_front(threads_ready);
        } while (next == (thread_control_block *)0);

        current_running_tcb = task;
        if (next != current_running_tcb)
            switch_to_thread(&current_running_tcb, next);
    }
}
void lock_postpone(void) {
    __asm__("cli");
    IRQ_disable_counter++;
    postpone_task_switches_counter++;
}
void unlock_postpone_and_schedule(void) {
    postpone_task_switches_counter--;
    if(postpone_task_switches_counter == 0) {
        if(task_switches_postponed_flag != 0) {
            task_switches_postponed_flag = 0;
            schedule();
        }
    }
    IRQ_disable_counter--;
    if(IRQ_disable_counter == 0)
        __asm__("sti");
}

void block_task(thread_status reason) {
    lock_postpone();
    {
        current_running_tcb->state = reason;
        schedule();
    }
    unlock_postpone_and_schedule();
}
void unblock_task(thread_control_block * task) {
    lock_postpone();
    {
        list_thread_push_back(threads_ready, task);
        task->state = THREAD_RUNNING;
    }
    unlock_postpone_and_schedule();
}
