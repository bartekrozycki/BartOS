
#include "thread_schelude.h"
#include "kernel_panic.h"
#include "print.h"
#include "kernel_heap_malloc.h"
#include "list_t.h"

#define GET_CR3(to) __asm__ ("movl %%cr3, %0": "=r"(to):)
#define SET_CR3(to) __asm__ ("movl %0, %%cr3": : "a"(to))

DELEGATE_LIST(task)

static task *current_task = 0x0;
static List *task_list = 0x0;
static u32 task_initialized = 0;

void bootstrap()
{
    BOCHS_BREAK;
    current_task->status = RUNNING;
    int status = (current_task->eip)();
    if (status == 0)
        print(TERMINAL, "PID %d ended\n", current_task->pid);
    print(TERMINAL, "kill stat %d", list_task_remove(task_list, current_task->pid));
}

void init_task(int (*task_ptr)(void))
{
    __asm__ ("cli");

    task_list = list_task_create();

    task idle;
    idle.stack_top = (u32) malloc(0x100000);
    idle.esp = idle.stack_top - 0x100000;
    GET_CR3(idle.cr3);
    idle.eip = (int (*)(void)) task_ptr;
    idle.pid = list_task_uid_iterator;
    idle.status = NEVER;

    list_task_add_begin(task_list, idle);

    current_task = &list_task_at(task_list, list_task_uid_iterator - 1)->data;

    print(TERMINAL, "Kernel IDLE PIT=%d initalized :D\n", current_task->pid);

    __asm__ ("sti");
}


static void schedule()
{
    if (task_list->head == NULL) print(TERMINAL, "TASK LIST NIL");
    return;
}

void task_switch(InterruptSave *is)
{
    if (!current_task) return;

    u32 cr3;
    GET_CR3(cr3);

    if (task_initialized == 0)
    {
        is->esp = current_task->esp;

        if (current_task->status == NEVER)
            is->eip = (u32) bootstrap;
        else
            is->eip = (u32) current_task->eip;

        if (cr3 != current_task->cr3)
        {
            current_task->cr3 = cr3;
            SET_CR3(cr3);
        }
        task_initialized = 1;
        return;
    }

    current_task->esp = is->esp;
    current_task->eip = (int (*)(void)) is->eip;

    schedule();

    is->esp = current_task->esp;

    if (current_task->status == NEVER)
        is->eip = (u32) bootstrap;
    else
        is->eip = (u32) current_task->eip;

    if (cr3 != current_task->cr3)
    {
        current_task->cr3 = cr3;
        SET_CR3(cr3);
    }

}