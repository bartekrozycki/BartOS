#include "threads.h"
#include "kernel_heap_malloc.h"

#define GET_CR3(to) __asm__ ("movl %%cr3, %0": "=r"(to):)
#define MIB(x) (x * 0x400 * 0x400)

struct thread_stack_t {
    u32 ebp;
    u32 edi;
    u32 esi;
    u32 ebx;
    u32 eip_ret1;
    u32 eip_ret2;
    u32 eip_ret3;
};

static u32 thread_create_counter = 1; // thats bad

void thread_free(thread_control_block *tcb)
{
    free(tcb->kbp_queue->sync);
    free(tcb->kbp_queue); // TODO destroy queue ~ memory leak
    free((void *) tcb->stack_alloc_ptr);
    free(tcb);
}
thread_control_block *thread_create(int (*func)(void))
{
    thread_control_block *thread = calloc(1, sizeof(thread_control_block));
    if(!thread) return NULL;

    thread->stack_alloc_ptr = (u32) calloc(MIB(1), 1); // TODO handle calloc == NULL
    thread->kbp_queue = (kbp_queue *) calloc(1, sizeof(struct kbp_queue_t));
    thread->kbp_queue->sync = (SEMAPHORE *) calloc(1, sizeof(struct SEMAPHORE_T));

    thread->esp = (u32) (thread->stack_alloc_ptr + MIB(1));
    thread->esp -= sizeof(struct thread_stack_t);

    thread->state = THREAD_RUNNING;
    thread->pid = thread_create_counter++;

    struct thread_stack_t *ptr = (struct thread_stack_t *) (thread->esp);
    ptr->ebp = 0x0;
    ptr->edi = 0x0;
    ptr->esi = 0x0;
    ptr->ebx = 0x0;

    ptr->eip_ret1 = (u32) thread_entry;
    ptr->eip_ret2 = (u32) func;
    ptr->eip_ret3 = (u32) thread_exit;

    GET_CR3(thread->cr3);

    lock_postpone();
    {
        list_thread_push_back(threads_ready, thread);
    }
    unlock_postpone_and_schedule();

    return thread;
}

