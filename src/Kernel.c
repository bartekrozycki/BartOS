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

_Noreturn int test1(void)
{
    print(TERMINAL, "Test 1\n");
    while (1)
    {
	   lock_key_queue();
		KBP *packet = key_queue_pop();

		print(TERMINAL, "%c - %d \n", packet->key_code);

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

	current_focus_tcb = thread_create(test1);

    butler_mr_cleaner();
}

