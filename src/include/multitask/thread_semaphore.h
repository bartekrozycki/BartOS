//
// Created by bartosz on 02.01.2021.
//

#pragma once

#include "thread_list.h"

typedef struct {
    int max_count;
    int current_count;
    thread_control_block *first_waiting_task;
    thread_control_block *last_waiting_task;
} SEMAPHORE;

SEMAPHORE *create_semaphore(int max_count);
SEMAPHORE *create_mutex(void);

void lock_semaphore(SEMAPHORE *semaphore);
void lock_mutex(SEMAPHORE *semaphore);

void unlock_semaphore(SEMAPHORE *semaphore);
void unlock_mutex(SEMAPHORE *semaphore);
