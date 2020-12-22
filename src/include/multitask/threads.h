#pragma once

#include <interrupts_handlers.h>
#include <ints.h>
#include "thread_list.h"

///// !!!!!!!!!!!!!!!!
//extern i32 IRQ_disable_counter;

extern thread_control_block *current_running_tcb;

extern thread_list *threads_ready;
extern thread_list *threads_sleeping;
extern thread_list *threads_terminated;

extern volatile i32 last_read_tick;
extern volatile i32 idleCPUTime;
extern volatile i32 timer_tick;
extern volatile i32 last_read_tick;
///// !!!!!!!!!!!!!!!!

extern void switch_to_thread (thread_control_block **current_thread,
                              thread_control_block *next_thread) __attribute__((cdecl));

void init_task();


void schedule(void);

void lock_scheduler(void);
void unlock_scheduler(void);

void block_task(thread_status reason);
void unblock_task(thread_control_block * task);

void lock_postpone(void);
void unlock_postpone(void);

void mili_sleep(i32 mili_seconds);
void sleep(i32 seconds);

void update_time_used(void);

extern void pit_interrupt(InterruptSave *is);

void thread_entry();
void thread_exit(void);

thread_control_block *thread_create(int (*eip)(void));

int test_task2(void);
