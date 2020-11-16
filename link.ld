ENTRY(_start)

KERNEL_BOOT_VMA = 0x00100000;
KERNEL_HIGH_VMA = 0xE0000000; /* _kernel_start */

SECTIONS
{
	. = KERNEL_BOOT_VMA;

	.boot ALIGN(4K) : { 
		*/Boot.o (.text)
		*/boot_memory_init.o (.*)
		. = ALIGN(0x1000);
	}  

	. += KERNEL_HIGH_VMA;

	_kernel_start = .;

	.text ALIGN(4K) : AT( ADDR(.text) - KERNEL_HIGH_VMA)
	{
		*(EXCLUDE_FILE (*/Boot.o */boot_memory_init.o) .text)
		*(EXCLUDE_FILE (*/boot_memory_init.o) .rodata)
	}
	.data ALIGN(4K) : AT( ADDR(.data) - KERNEL_HIGH_VMA)
	{
		*(EXCLUDE_FILE (*/boot_memory_init.o) .data)
	}
	.bss ALIGN(4K) : AT( ADDR(.bss) - KERNEL_HIGH_VMA)
	{
		*(EXCLUDE_FILE (*/boot_memory_init.o) COMMON)
		*(EXCLUDE_FILE (*/boot_memory_init.o) .bss)
		. = ALIGN(0x1000);
	}
	_kernel_end = .;

	. += 3M - (_kernel_end - _kernel_start) - 0x1000;

	_init_mem_end = .;
}
