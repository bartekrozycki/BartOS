#include "threads.h"


void thread_entry(int (*eip)(void))
{
    running_thread->status = THREAD_ACTIVE;
    unlock_scheduler();
    eip();
}

void thread_exit(void)
{

}