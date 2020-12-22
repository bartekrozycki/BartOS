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


int test_task2(void)
{
    u32 counter = 0;
    for (; counter < 5;)
    {
        lock_postpone();
        print_at(TERMINAL, 0, 0, "Timer every 1000ms %d", counter);
        unlock_postpone();
        sleep(1);
        counter++;
    }

    return 0;
}

_Noreturn void idle(void)
{
    u32 counter = 0;
    for (;;)
    {
        lock_postpone();
        print_at(TERMINAL, 25, 0, "Timer every 200ms %d", counter);
        unlock_postpone();
        mili_sleep(200);
        counter++;
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

    idle();
}

