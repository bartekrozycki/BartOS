#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "terminal.h"
#include "misc.h"
#include "print.h"
#include "keyboard.h"

void Main(u32 mboot_magic, MultibootInfo* mboot_info)
{
	if (mboot_magic != MULTIBOOT_EAX_MAGIC)
		permahalt();

	terminal_initialize();

	MultibootMemoryMap 	*mmap = (void *) mboot_info->mmap_address; // 0x10b844
	// MultibootModules 	*modules = (void *) mboot_info->mods_address;

	gdt_init();
	idt_init();

	keyboard_init();

	int_wait_forever();
}
 