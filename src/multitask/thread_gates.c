#include "threads.h"

void thread_entry()
{
    unlock_scheduler();
}

void thread_exit(void)
{
    terminate_task();
}