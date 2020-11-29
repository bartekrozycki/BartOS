#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "print.h"
#include "kernel_heap.h"
#include "kernel_panic.h"
#include "terminal.h"
#include "keyboard.h"

_Noreturn void Main(MultibootInfo *mbi) // TODO mbi pointer is not secure --> pointing low physical memory
{
	init_gdt();
	init_idt();
	init_paging();
	
	init_heap();
	init_keyboard();

	init_terminal();

	print(TERMINAL, "\n  ____             _    ____   _____ \n");
	print(TERMINAL, " |  _ \\           | |  / __ \\ / ____|\n");
	print(TERMINAL, " | |_) | __ _ _ __| |_| |  | | (___  \n");
	print(TERMINAL, " |  _ < / _` | '__| __| |  | |\\___ \\ \n");
	print(TERMINAL, " | |_) | (_| | |  | |_| |__| |____) |\n");
	print(TERMINAL, " |____/ \\__,_|_|   \\__|\\____/|_____/ \n\n");\
	print(TERMINAL, "\n > ");

	kPanic;

    __asm__("sti");
    while (1)
        __asm__("hlt");
}
