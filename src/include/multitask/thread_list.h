#pragma once

#include "ints.h"
#include "keyboard.h"

typedef enum thread_status_t {
    THREAD_RUNNING,
    THREAD_PAUSED,
    THREAD_SLEEPING,
    THREAD_TERMINATED, // task rip
} thread_status;

typedef struct thread_control_block_t {
    u32 esp;
    u32 cr3;
    struct thread_control_block_t *next;
    thread_status state;
    i32 paused_until;
    i32 time_used;
    u32 pid;
    u32 stack_alloc_ptr;
    struct kbp_queue_t *kbp_queue;
} thread_control_block;

typedef struct thread_list_t {
    thread_control_block *head;
    thread_control_block *tail;
    u32 size;
} thread_list;

thread_list * list_thread_create(void);

u32 list_thread_push_front(thread_list *list, thread_control_block *ptr);
u32 list_thread_push_back(thread_list *list, thread_control_block *ptr);
thread_control_block * list_thread_pop_front(thread_list *list);
thread_control_block * list_thread_pop_back(thread_list *list);
thread_control_block * list_thread_remove(thread_list *list, thread_control_block *ptr);
void list_thread_display(thread_list *list);

