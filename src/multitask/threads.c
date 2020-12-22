#include "threads.h"
#include "kernel_panic.h"
#include "print.h"
#include "kernel_heap_malloc.h"
#include "pit.h"

#define GET_CR3(to) __asm__ ("movl %%cr3, %0": "=r"(to):)

typedef thread_control_block* TCB_Array;

thread_control_block *current_running_tcb;
thread_control_block *kernel_thread;

thread_list *threads_ready;
thread_list *threads_sleeping;
thread_list *threads_terminated;

int siema(void)
{
    u32 counter = 0;
    for (;;)
    {
        lock_postpone();
        print_at(TERMINAL, 50, 0, "Timer every 200ms %d", counter);
        unlock_postpone();
        mili_sleep(200);
        counter++;
    }
}
void init_task()
{
    threads_ready = list_thread_create();
    threads_sleeping = list_thread_create();
    threads_terminated = list_thread_create();

    current_running_tcb = malloc(sizeof(thread_control_block)); // kernel
    GET_CR3(current_running_tcb->cr3);
    current_running_tcb->pid = 0; // kernel
    current_running_tcb->state = THREAD_RUNNING;
    kernel_thread = current_running_tcb;

    thread_control_block *first  = thread_create(test_task2);
    first->pid = 1;
    thread_control_block *second  = thread_create(siema);
    second->pid = 2;
}


