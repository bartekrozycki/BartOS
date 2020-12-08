#pragma once

#include <interrupts_handlers.h>
#include <ints.h>

typedef enum task_status {
    EXITED, RUNNING, SLEEPING, NEVER
} task_status;

typedef struct task{
    task_status status;
    u32 pid;
    u32 esp;
    u32 stack_top;
    int (*eip)(void);
    u32 cr3;
} task;

void init_task(int (*task_ptr)(void));

int kernel_idle(void);

void task_switch(InterruptSave *is);
