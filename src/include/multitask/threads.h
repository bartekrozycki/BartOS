#pragma once

#include <interrupts_handlers.h>
#include <ints.h>
#include "thread_list.h"

///// !!!!!!!!!!!!!!!!
//extern i32 IRQ_disable_counter;


extern i32 IRQ_disable_counter;
extern i32 postpone_task_switches_counter;
extern i32 task_switches_postponed_flag;

extern thread_control_block *current_running_tcb;
extern thread_control_block *current_focus_tcb;

extern thread_list *threads_ready;
extern thread_list *threads_sleeping;
extern thread_list *threads_terminated;

extern volatile i32 last_read_tick;
extern volatile i32 idleCPUTime;
extern volatile i32 timer_tick;
extern volatile i32 last_read_tick;

extern thread_control_block *butler;
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
void unlock_postpone_and_schedule(void);

void mili_sleep(i32 mili_seconds);
void sleep(i32 seconds);

void update_time_used(void);

extern void pit_interrupt(InterruptSave *is);

void terminate_task(void);

void thread_entry();
void thread_exit(void);

thread_control_block *thread_create(int (*func)(void));
void thread_free(thread_control_block *tcb);

_Noreturn void butler_mr_cleaner(void);

int test_task2(void);
