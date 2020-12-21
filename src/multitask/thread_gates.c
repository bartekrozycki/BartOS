#include "threads.h"


void thread_entry(int (*eip)(void))
{
    unlock_scheduler();
    eip();
}

void thread_exit(void)
{

}