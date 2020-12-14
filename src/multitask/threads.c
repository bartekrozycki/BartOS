#include "threads.h"
#include "kernel_panic.h"
#include "print.h"
#include "kernel_heap_malloc.h"
#include "pit.h"

#define GET_CR3(to) __asm__ ("movl %%cr3, %0": "=r"(to):)

thread_control_block *running_thread;

thread_list *active;
thread_list *waiting;
thread_list *terminated;

void init_task(int (*task_ptr)(void))
{
    active = list_thread_create();
    waiting = list_thread_create();
    terminated = list_thread_create();

    running_thread = malloc(sizeof(thread_control_block));
    GET_CR3(running_thread->cr3);
    running_thread->pid = 0; // kernel

    thread_control_block *test_task  = thread_create(task_ptr);
    test_task->pid = 1;
    thread_control_block *test_task22  = thread_create(test_task2);
    test_task22->pid = 2;

    list_thread_push_back(active, test_task);
    list_thread_push_back(active, test_task22);

    for (;;)
    {
        lock_scheduler();
        print(TERMINAL, "xxxx jebac %d\n", IRQ_disable_counter);
        schedule();
        unlock_scheduler();
    }
}


