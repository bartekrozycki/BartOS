#include "threads.h"
#include "kernel_panic.h"
#include "print.h"
#include "kernel_heap_malloc.h"
#include "pit.h"

#define GET_CR3(to) __asm__ ("movl %%cr3, %0": "=r"(to):)

typedef thread_control_block* TCB_Array;

thread_control_block *running_thread;
thread_control_block *kernel_thread;

thread_list *threads_ready;
thread_list *threads_sleeping;
thread_list *threads_terminated;



void init_task()
{
    threads_ready = list_thread_create();
    threads_sleeping = list_thread_create();
    threads_terminated = list_thread_create();

    running_thread = malloc(sizeof(thread_control_block)); // idle task
    GET_CR3(running_thread->cr3);
    running_thread->pid = 0; // kernel
    kernel_thread = running_thread;

    thread_control_block *test_task22  = thread_create(test_task2);
    test_task22->pid = 1;

    list_thread_push_back(threads_ready, test_task22);
}


