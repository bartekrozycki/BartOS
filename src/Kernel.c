#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "print.h"
#include "kernel_heap.h"
#include "kernel_panic.h"
#include "terminal.h"
#include "keyboard.h"
#include "pit.h"
#include "thread_schelude.h"

int kernel_idle(void)
{

    for (int i = 0; i < 500; ++i)
        print(TERMINAL, "%d \n", i);

    return 0;
}

_Noreturn void Main(MultibootInfo *mbi)
{
	init_gdt();
	init_idt(); /// interrupts need "sti"

	*mbi = *mbi;

	/// !!!! mbi can be used ONLY above !!!!
	/// init_paging removing lower memory map :)

	init_paging();
	init_heap();

	init_keyboard();
	init_terminal();

    init_pit(1000);

    ///http://patorjk.com/software/taag/

    print(TERMINAL, "Kernel Main Function started.\n");

    init_task(kernel_idle);

    __asm__("sti");
    while (1)
        __asm__("hlt");
}
