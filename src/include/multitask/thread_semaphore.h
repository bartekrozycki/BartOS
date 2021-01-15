//
// Created by bartosz on 02.01.2021.
//

#pragma once

#include "thread_list.h"

typedef struct SEMAPHORE_T{
    int max_count;
    int current_count;
    struct thread_control_block_t *first_waiting_task;
    struct thread_control_block_t *last_waiting_task;
} SEMAPHORE;

SEMAPHORE *create_semaphore(int max_count);
SEMAPHORE *create_mutex(void);

void lock_semaphore(SEMAPHORE *semaphore);
void lock_mutex(SEMAPHORE *semaphore);

void unlock_semaphore(SEMAPHORE *semaphore);
void unlock_mutex(SEMAPHORE *semaphore);

void wait_semaphore(SEMAPHORE *semaphore);
void post_semaphore(SEMAPHORE *semaphore);
