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

// scheduler should be locked before calling
void schedule(void)
{
    if(postpone_task_switches_counter != 0) {
        task_switches_postponed_flag = 1;
        return;
    }
    if (running_thread != NULL)
    {
        thread_control_block *thread = list_thread_pop_front(threads_ready);
        if (running_thread->state == THREAD_PAUSED)
            list_thread_push_back(threads_sleeping, running_thread);
        else
            list_thread_push_back(threads_ready, running_thread);

        switch_to_thread(&running_thread, thread);
    }
}

void block_task(thread_status reason) {
    lock_scheduler();
    running_thread->state = THREAD_PAUSED;
    schedule(); // schedule push thread to paused list if state == thread_paused
    unlock_scheduler();
}
void unblock_task(thread_control_block * task) {
    lock_scheduler();

    if (list_thread_remove(threads_sleeping, task) == NULL)
        k_panic("unblocking not waiting thread ?");

    if((threads_ready->size < 1) || (running_thread->pid == 0)) {
        list_thread_push_back(threads_ready, running_thread);
        switch_to_thread(&running_thread, task);
    }
    else
        list_thread_push_back(threads_ready, task);

    unlock_scheduler();
}
void lock_stuff(void) {
    __asm__("cli");
    IRQ_disable_counter++;
    postpone_task_switches_counter++;
}
void unlock_stuff(void) {
    postpone_task_switches_counter--;
    if(postpone_task_switches_counter == 0) {
        if(task_switches_postponed_flag != 0) {
            task_switches_postponed_flag = 0;
            schedule();
        }
    }
    IRQ_disable_counter--;              // unlock scheduler
    if(IRQ_disable_counter == 0) {      // unlock scheduler
        __asm__("sti");                 // unlock scheduler
    }                                   // unlock scheduler
}
