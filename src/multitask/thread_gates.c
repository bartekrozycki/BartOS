#include "threads.h"
#include "print.h"


void thread_entry()
{
    unlock_scheduler();
}

void thread_exit(void)
{
    print(TERMINAL, "  - - - [ dead kurwa twoja stara nie zyje ] - - - ");
    block_task(THREAD_TERMINATED);
}