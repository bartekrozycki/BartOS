#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "print.h"
#include "kernel_heap.h"
#include "kernel_panic.h"
#include "terminal.h"
#include "keyboard.h"
#include "pit.h"
#include "threads.h"
#include "thread_semaphore.h"
#include "acpi.h"

SEMAPHORE *test;
int test1(void)
{
    for (int i = 0; i < 10; i++)
    {
        lock_mutex(test);
        mili_sleep(500);
        print(TERMINAL,"Ala ma ");
        unlock_mutex(test);
    }
    return 0;
}
int test2(void)
{
    for (int i = 0; i < 10; i++)
    {
        lock_mutex(test);
        print(TERMINAL,"kota\n");
        unlock_mutex(test);
    }
    return 0;
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

	lock_mutex(test);
    print(TERMINAL, "Creating tasks.. start\n");
    print(TERMINAL, "task #1\n");
    thread_create(test1);
    print(TERMINAL, "#1 created\n");

    print(TERMINAL, "Creating task #2..\n");
    thread_create(test2);
    print(TERMINAL, "Creating task #2 created..\n");

    print(TERMINAL, "Creating tasks.. end\n");
    unlock_mutex(test);


    butler_mr_cleaner();
}

