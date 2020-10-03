#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "terminal.h"
#include "misc.h"
#include "print.h"
#include "keyboard.h"
#include "serial.h"

extern u32 stack_top;
extern u32 _kernel_end;

void Main(u32 mboot_magic, MultibootInfo* mboot_info)
{
	if (mboot_magic != MULTIBOOT_EAX_MAGIC)
		permahalt();

	init_serial();
	init_terminal();

	if (mboot_info->flags & 0b1000001 ) // verify mmap and memlowwer&memupper loaded correctly
	{
		MultibootMemoryMap 	*mmap = (MultibootMemoryMap *) mboot_info->mmap_address;

		while ((u32) mmap < (mboot_info->mmap_address + mboot_info->mmap_length))
		{
				/////////////////////////////////////////
	//////////// TODO memory managment paging ect
				////////////////////////
			mmap = (MultibootMemoryMap *)((u32)mmap + mmap->size + sizeof(mmap->size));
		}
	}
	else permahalt();

	init_gdt();
	init_idt();
	keyboard_init();

	int_wait_forever();
}
 