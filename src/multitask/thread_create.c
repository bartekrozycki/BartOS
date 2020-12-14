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
    u32 eip_ret3;
    u32 eip_ret2;
};

thread_control_block *thread_create(int (*eip)(void))
{
    thread_control_block *thread = malloc(sizeof(thread_control_block));
    if(!thread) return NULL;

    thread->esp = (u32) calloc(MIB(1), 1);
    thread->esp += MIB(5);
    thread->esp -= sizeof(struct thread_stack_t);
    thread->status = THREAD_NEW;

    struct thread_stack_t *ptr = (struct thread_stack_t *) (thread->esp);
    ptr->ebp = 0x0;
    ptr->edi = 0x0;
    ptr->esi = 0x0;
    ptr->ebx = 0x0;

    ptr->eip_ret1 = (u32) thread_entry;
    ptr->eip_ret2 = (u32) eip;
    ptr->eip_ret3 = (u32) thread_exit;

    GET_CR3(thread->cr3);

    return thread;
}

