AS        = nasm
ASFLAGS   = -f elf32

CROSS = ~/opt/cross/bin

CC	= $(CROSS)/i686-elf-gcc
CFLAGS	= -ffreestanding -nostdlib -nostdinc
CFLAGS	+= -Wall -Wextra -std=c99 -O0 -g
CFLAGS	+= -Isrc/include
CFLAGS	+= -Isrc/include/std
CFLAGS	+= -Isrc/include/drivers
CFLAGS	+= -Isrc/include/memory
CFLAGS	+= -Isrc/include/interrupts
CFLAGS  += -Isrc/include/data_structures
CFLAGS  += -Isrc/include/multitask


ifeq ($(debug), 1)
$(warning -=  =- -=  =- -=  =-)
$(warning Debug mode)
$(warning -=  =- -=  =- -=  =-)
CFLAGS 	+= -DDEBUG=1
else
$(warning -=  =- -=  =- -=  =-)
$(warning Compile mode)
$(warning -=  =- -=  =- -=  =-)
endif


LD		  = $(CROSS)/i686-elf-ld
LINKER    = link.ld
LDFLAGS   = -T $(LINKER) -Map memory.map

ISO       = os.iso
KERNEL    = kernel.bin

C_FILES   = $(shell find src/ -type f -name "*.c")
H_FILES   = $(shell find src/ -type f -name "*.h")
ASM_FILES = $(shell find src/ -type f -name "*.asm")

C_OBJS    = $(C_FILES:.c=.o)
ASM_OBJS  = $(ASM_FILES:.asm=.o)

OBJS      = $(C_OBJS) $(ASM_OBJS)

.PHONY: all clean

all: $(ISO) clean

${ISO}: ${KERNEL}
	@mkdir -p isodir/boot/grub
	@mv $(KERNEL) isodir/boot/$(KERNEL)
	@cp grub.cfg isodir/boot/grub/grub.cfg
	@grub-mkrescue -o os.iso isodir

${KERNEL}: $(OBJS) $(LINKER)
	$(info Linking object files)
	@$(LD) $(LDFLAGS) $(OBJS) -o $@

%.o: %.asm
	$(info [Compiling ASM] $^)
	@$(AS) $(ASFLAGS) $^
%.o: %.c
	$(info [Compiling C] $^)
	@$(CC) $(CFLAGS) -c $^ -o $@

clean:
	$(info Cleaning)
	@rm -f $(OBJS)
run-terminal-bochs:
	@gnome-terminal -- bochs
run-terminal-qemu:
	@gnome-terminal -- qemu-system-i386 os.iso -m 256

