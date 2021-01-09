#include "threads.h"
#include "thread_list.h"
#include "print.h"

void terminate_task(void) {

    // Note: Can do any harmless stuff here (close files, free memory in user-space, ...) but there's none of that yet

    lock_postpone();
    {
        list_thread_push_back(threads_terminated, current_running_tcb);
        block_task(THREAD_TERMINATED);
        unblock_task(butler);
    }
    unlock_postpone_and_schedule();
}

void cleanup_terminated_task(thread_control_block * task) {

    thread_free(task);
}
_Noreturn void butler_mr_cleaner(void) {
    for (;;)
    {
        lock_postpone(); // +1 +1 flaga 0
        {
            if (threads_terminated->head == NULL) // 1 1 FLAG 0
            {
                block_task(THREAD_PAUSED); // 1 1 flag 1
                unlock_postpone_and_schedule(); // 0 1 flag 1
                continue;
            }

            thread_control_block *work = list_thread_pop_front(threads_terminated);
            cleanup_terminated_task(work);
        }
        unlock_postpone_and_schedule(); // -1 -1 flaga 0
    }
}
