#include "threads.h"
#include "print.h"

i32 IRQ_disable_counter = 0;

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
    if (running_thread != NULL)
    {
        thread_control_block *thread = list_thread_pop_front(active);
        list_thread_push_back(active, running_thread);
        switch_to_thread(&running_thread, thread);
    }
}
