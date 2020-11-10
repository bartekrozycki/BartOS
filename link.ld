ENTRY(_start)

KERNEL_BOOT_VMA = 0x00100000;
KERNEL_HIGH_VMA = 0xE0000000; /* 3.5 GiB :D */

SECTIONS
{
	. = KERNEL_BOOT_VMA;
	.boot :{ 
		*/Boot.o (.text)
		*/memory_manager.o (.*)
	}  

	. += KERNEL_HIGH_VMA;

	_kernel_start = .;

	.text ALIGN(4K) : AT( ADDR(.text) - KERNEL_HIGH_VMA)
	{
		*(EXCLUDE_FILE (*/Boot.o */memory_manager.o) .text)
		*(EXCLUDE_FILE (*/memory_manager.o) .rodata)
	}
	.data ALIGN(4K) : AT( ADDR(.data) - KERNEL_HIGH_VMA)
	{
		*(EXCLUDE_FILE (*/memory_manager.o) .data)
	}
	.bss ALIGN(4K) : AT( ADDR(.bss) - KERNEL_HIGH_VMA)
	{
		*(EXCLUDE_FILE (*/memory_manager.o) COMMON)
		*(EXCLUDE_FILE (*/memory_manager.o) .bss)
		. = ALIGN(0x1000);
	}
	_kernel_end = .;
}
