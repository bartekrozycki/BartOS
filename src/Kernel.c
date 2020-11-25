#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "system.h"
#include "print.h"
#include "heap.h"
#include "kernel_panic.h"
#include "terminal.h"
#include "keyboard.h"

void Main(MultibootInfo *mbi) // TODO mbi pointer is not secure --> pointing low physical memory
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
	print(TERMINAL, " Zalety: \n");
	print(TERMINAL, " - system jest bardzo szybki\n\n");
	print(TERMINAL, " Funkcjonalnosci: \n");
	print(TERMINAL, " - mozna pisac\n");
	print(TERMINAL, " - koniec xD\n\n > ");

	int_wait_forever();
}
