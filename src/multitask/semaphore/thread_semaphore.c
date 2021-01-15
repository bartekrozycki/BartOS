//
// Created by bartosz on 02.01.2021.
//

#include "thread_semaphore.h"
#include "threads.h"
#include "kernel_heap_malloc.h"
#include "print.h"

//
//  create semaphore & mutex
//
SEMAPHORE *create_semaphore(int max_count)
{
    SEMAPHORE *semaphore = malloc(sizeof(SEMAPHORE));

    if (!semaphore) return NULL;

    semaphore->max_count = max_count;
    semaphore->current_count = 0;
    semaphore->first_waiting_task = NULL;
    semaphore->last_waiting_task = NULL;

    return  semaphore;
}
SEMAPHORE *create_mutex(void)
{
    return create_semaphore(1);
}

//
//  lock semaphore & mutex
//
void wait_semaphore(SEMAPHORE *semaphore)
{
    lock_postpone();
    {
        if (--semaphore->current_count < semaphore->max_count) {

            current_running_tcb->next = NULL;

            if (semaphore->first_waiting_task == NULL)
                semaphore->first_waiting_task = current_running_tcb;
            else
                semaphore->last_waiting_task->next = current_running_tcb;

            semaphore->last_waiting_task = current_running_tcb;
            block_task(THREAD_PAUSED);
        }
    }
    unlock_postpone_and_schedule();
}
void lock_semaphore(SEMAPHORE *semaphore) {
    lock_postpone();
    {
        if (semaphore->current_count >= semaphore->max_count) {
            current_running_tcb->next = NULL;

            if (semaphore->first_waiting_task == NULL)
                semaphore->first_waiting_task = current_running_tcb;
            else
                semaphore->last_waiting_task->next = current_running_tcb;

            semaphore->last_waiting_task = current_running_tcb;
            block_task(THREAD_PAUSED);
        } else ++semaphore->current_count;
    }
    unlock_postpone_and_schedule();
}
void lock_mutex(SEMAPHORE *semaphore)
{
    lock_semaphore(semaphore);
}

//
//  unlock semaphore & mutex
//
void post_semaphore(SEMAPHORE *semaphore) {
    lock_postpone();
    {
        if (semaphore->first_waiting_task) {
            thread_control_block *task = semaphore->first_waiting_task;
            semaphore->first_waiting_task = task->next;
            ++semaphore->current_count;
            unblock_task(task);
        }
    }
    unlock_postpone_and_schedule();
}
void unlock_semaphore(SEMAPHORE *semaphore) {
    lock_postpone();
    {
        if (semaphore->first_waiting_task) {
            thread_control_block *task = semaphore->first_waiting_task;
            semaphore->first_waiting_task = task->next;
            unblock_task(task);
        } else --semaphore->current_count;
    }
    unlock_postpone_and_schedule();
}
void unlock_mutex(SEMAPHORE *semaphore)
{
    unlock_semaphore(semaphore);
}