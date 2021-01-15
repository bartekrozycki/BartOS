#include "multiboot.h"
#include "gdt.h"
#include "interrupt_descriptor.h"
#include "print.h"
#include "kernel_heap.h"
#include "kernel_panic.h"
#include "vga.h"
#include "keyboard.h"
#include "pit.h"
#include "threads.h"
#include "thread_semaphore.h"
#include "acpi.h"
#include "kernel_heap_malloc.h"

SEMAPHORE *test;

#define IS_WRITTABLE(x) ( (x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z') || x == ' '|| x == '\n')

thread_control_block *t1, *t2;
_Noreturn int test1(void)
{
    size_t line = 2;
    while (1)
    {
        print_at(TERMINAL, 0,0, "WAITING 1\n");
        wait_semaphore(t1->kbp_queue->sync);
        KBP *packet = key_queue_pop();

        if(packet->key_code == KEY_RELEASED_f2)
        {
            print_at(TERMINAL,0,1, "TEST 1 %p\n", current_focus_tcb);
            lock_scheduler();
            {
                current_focus_tcb = t2;
            }
            unlock_scheduler();
            mili_sleep(500);
            print_at(TERMINAL,0,1, "TEST 1 changed %p\n", current_focus_tcb);
        }

        print_at(TERMINAL,0,line++, "%c %b\n", packet->unicode_code, *packet->status);

        free(packet);
    }
}
_Noreturn int test2(void)
{
    size_t line = 2;
    while (1)
    {
        print_at(TERMINAL, 40,0, "WAITING 2 %p\n", current_focus_tcb);
        wait_semaphore(t2->kbp_queue->sync);
        KBP *packet = key_queue_pop();

        if(packet->key_code == KEY_RELEASED_f1)
        {
            print_at(TERMINAL,40,1, "TEST 2\n");
            lock_scheduler();
            {
                current_focus_tcb = t1;
            }
            unlock_scheduler();
        }

        print_at(TERMINAL,40,line++, "%c %b\n", packet->unicode_code, *packet->status);

        free(packet);
    }
}

_Noreturn void Main(MultibootInfo *mbi) // DO NOT USE MBI BECAUSE IT IS NOT MAPPED:)
{
	init_gdt();
	init_idt(); /// interrupts need "sti"

	init_paging();
	init_heap();

	init_keyboard();
	init_terminal();
	init_acpi();

    init_task();
	init_pit(1000);

	test = create_mutex();

    t2 = thread_create(test2);
    t1 = thread_create(test1);

    current_focus_tcb = t1;

    butler_mr_cleaner();
}

