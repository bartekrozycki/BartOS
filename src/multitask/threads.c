#include "threads.h"
#include "kernel_panic.h"
#include "print.h"
#include "kernel_heap_malloc.h"

#define GET_CR3(to) __asm__ ("movl %%cr3, %0": "=r"(to):)

typedef thread_control_block* TCB_Array;

thread_control_block *current_running_tcb;
thread_control_block *butler;

thread_list *threads_ready;
thread_list *threads_sleeping;
thread_list *threads_terminated;


void init_task()
{
    threads_ready = list_thread_create();
    threads_sleeping = list_thread_create();
    threads_terminated = list_thread_create();

    current_running_tcb = malloc(sizeof(thread_control_block)); // kernel
    GET_CR3(current_running_tcb->cr3);
    current_running_tcb->pid = 0; // kernel
    current_running_tcb->state = THREAD_RUNNING;

    butler = current_running_tcb;
}


