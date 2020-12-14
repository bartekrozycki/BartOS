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
#include "acpi.h"


int test_task(void)
{

	for (;;)
	{
		lock_scheduler();
		print(TERMINAL, "xxxxxxxx pis %d\n", IRQ_disable_counter);
		schedule();
		unlock_scheduler();
	}

	return 0;
}
int test_task2(void)
{

	for (;;)
	{
		lock_scheduler();
		print(TERMINAL, "xxxxxxxxxxxx :) %d\n", IRQ_disable_counter);
		schedule();
		unlock_scheduler();
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

    init_pit(1000);
    init_task(test_task);

    print(TERMINAL, "Hello main");

}
