#pragma once

#include <interrupts_handlers.h>
#include <ints.h>
#include "thread_list.h"

///// !!!!!!!!!!!!!!!!
//extern i32 IRQ_disable_counter;

extern thread_control_block *running_thread;

extern thread_list *threads_ready;
extern thread_list *threads_sleeping;
extern thread_list *threads_terminated;
///// !!!!!!!!!!!!!!!!

extern void switch_to_thread
(thread_control_block **current_thread, thread_control_block *next_thread) __attribute__((cdecl));

void init_task();


void schedule(void);

void lock_scheduler(void);
void unlock_scheduler(void);

void block_task(thread_status reason);
void unblock_task(thread_control_block * task);

void lock_stuff(void);
void unlock_stuff(void);

void sleep(u32 seconds);

void thread_entry(int (*eip)(void));
void thread_exit(void);

thread_control_block *thread_create(int (*eip)(void));

int test_task2(void);
