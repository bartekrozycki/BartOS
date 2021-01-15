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
        print_at(TERMINAL, 0,0, "Thread 1 %p\n", current_focus_tcb);
        wait_semaphore(t1->kbp_queue->sync);
        KBP *packet = key_queue_pop();

        if(packet->key_code == KEY_RELEASED_f2)
        {
            lock_scheduler();
            {
                current_focus_tcb = t2;
            }
            unlock_scheduler();
        }

        if (IS_WRITTABLE(packet->unicode_code))
            print_at(TERMINAL,0,line < 24 ? line++ : line, "%c %b\n", packet->unicode_code, *packet->status);

        free(packet);
    }
}
_Noreturn int test2(void)
{
    size_t line = 2;
    while (1)
    {
        print_at(TERMINAL, 40,0, "Thread 2 %p\n", current_focus_tcb);
        wait_semaphore(t2->kbp_queue->sync);
        KBP *packet = key_queue_pop();

        if(packet->key_code == KEY_RELEASED_f1)
        {
            lock_scheduler();
            {
                current_focus_tcb = t1;
            }
            unlock_scheduler();
        }
        if (IS_WRITTABLE(packet->unicode_code))
            print_at(TERMINAL,40,line < 24 ? line++ : line, "%c %b\n", packet->unicode_code, *packet->status);

        free(packet);
    }
}

_Noreturn int test3(void)
{
    int time = 0;
    while (1)
    {
        print_at(TERMINAL, 30, 0, "TIMER %d", ++time);
        sleep(1);
        if (time % 5 == 0)
        {
            terminal_setcolor(TERMINAL_COLOR_BLUE_SCREEN_OF_DEATH);
        }
        if (time % 10 == 0)
        {
            terminal_setcolor(TERMINAL_COLOR_DOS_DEFAULT);
        }
    }
}
_Noreturn int test4(void)
{
    int time = 0;
    sleep(3);
    while (1)
    {
        print_at(TERMINAL, 20, 0, "TIMER %d", ++time);
        sleep(1);
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

    thread_create(test3);
    thread_create(test4);
    t2 = thread_create(test2);
    t1 = thread_create(test1);

    current_focus_tcb = t1;

    butler_mr_cleaner();
}

