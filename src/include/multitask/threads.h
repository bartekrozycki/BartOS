#pragma once

#include <interrupts_handlers.h>
#include <ints.h>
#include "thread_list.h"

///// !!!!!!!!!!!!!!!!
extern i32 IRQ_disable_counter;

extern thread_control_block *running_thread;

extern thread_list *active;
extern thread_list *waiting;
extern thread_list *terminated;
///// !!!!!!!!!!!!!!!!

extern void switch_to_thread(thread_control_block **current_thread, thread_control_block *next_thread);

void init_task(int (*task_ptr)(void));

void lock_scheduler(void);
void unlock_scheduler(void);
void schedule(void);

void update_time_used(void);

void thread_entry(int (*eip)(void));
void thread_exit(void);

thread_control_block *thread_create(int (*eip)(void));

int test_task2(void);