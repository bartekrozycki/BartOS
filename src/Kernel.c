#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "terminal.h"
#include "system.h"
#include "print.h"
#include "keyboard.h"
#include "serial.h"
#include "paging.h"
#include "memory.h"
#include "pit.h"

extern u32 _kernel_end; 
extern u32 _kernel_start;

void Main(u32 mboot_magic, MultibootInfo* mboot_info)
{
	if (mboot_magic != MULTIBOOT_EAX_MAGIC)
		permahalt();

	init_serial();
	init_terminal();

	init_memory(mboot_info);
	// init_paging();

	init_gdt();
	init_idt();

	init_pit();
	keyboard_init();


	print(TERMINAL, "\n  ____             _    ____   _____ \n");
	print(TERMINAL, " |  _ \\           | |  / __ \\ / ____|\n");
	print(TERMINAL, " | |_) | __ _ _ __| |_| |  | | (___  \n");
	print(TERMINAL, " |  _ < / _` | '__| __| |  | |\\___ \\ \n");
	print(TERMINAL, " | |_) | (_| | |  | |_| |__| |____) |\n");
	print(TERMINAL, " |____/ \\__,_|_|   \\__|\\____/|_____/ \n");
	print(TERMINAL, "\n > ");

	int_wait_forever();
}
 