#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "terminal.h"
#include "system.h"
#include "print.h"
#include "keyboard.h"
#include "serial.h"
#include "pit.h"
#include "kalloc.h"


void Main(MultibootInfo* mboot_info)
{
	mboot_info = (MultibootInfo *) mboot_info;////////////////

	init_gdt();
	init_idt();

	init_terminal();
	keyboard_init();

	// u32* ptr = kalloc();
	// print(TERMINAL, "Allocated: %p\n", ptr);
	// kfree((u32)ptr + 1);
	// print(TERMINAL, "Free /\\");


	print(TERMINAL, "\n  ____             _    ____   _____ \n");
	print(TERMINAL, " |  _ \\           | |  / __ \\ / ____|\n");
	print(TERMINAL, " | |_) | __ _ _ __| |_| |  | | (___  \n");
	print(TERMINAL, " |  _ < / _` | '__| __| |  | |\\___ \\ \n");
	print(TERMINAL, " | |_) | (_| | |  | |_| |__| |____) |\n");
	print(TERMINAL, " |____/ \\__,_|_|   \\__|\\____/|_____/ \n");
	print(TERMINAL, "\n > ");

	int_wait_forever();
}
 