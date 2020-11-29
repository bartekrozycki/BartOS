#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "system.h"
#include "print.h"
#include "kernel_heap.h"
#include "kernel_panic.h"
#include "terminal.h"
#include "keyboard.h"
#include "kernel_heap_malloc.h"

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
	print(TERMINAL, "\n > ");

    u32* ptr = malloc(1);
    u32* ptr1 = malloc(1);
    u32* ptr2 = malloc(1);
    u32* ptr3 = malloc(1);

    print(SERIAL, "%p\n%p\n%p\n%p\n", ptr, ptr1, ptr2, ptr3);

    free(ptr);
    free(ptr1);
    free(ptr2);
    free(ptr3);

    ptr = malloc(1);
    ptr1 = malloc(1);
    ptr2 = malloc(1);
    ptr3 = malloc(1);

    print(SERIAL, "%p\n%p\n%p\n%p\n", ptr, ptr1, ptr2, ptr3);

    free(ptr);
    free(ptr1);
    free(ptr2);
    free(ptr3);

	int_wait_forever();
}
