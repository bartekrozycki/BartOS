global _start:function (_start.end - _start)
global gdt_load
global load_idt
global permahalt

extern KERNEL_BOOT_VMA
extern KERNEL_HIGH_VMA

extern _kernel_start
extern _kernel_end


extern boot_init_mem

; Contstants for Multiboot
FLAGS	        equ	11b
MAGIC	        equ	0x1BADB002
CHKSUM	        equ	-(MAGIC + FLAGS)
INITSTACKSIZE	equ	0x8000 ; 1MiB

section .data
    align 0x1000
    kernel_stack times INITSTACKSIZE db 0

section .text
multiboot_header:
    align 4
    dd	MAGIC
    dd	FLAGS
    dd	CHKSUM

_start:

cli

; setup stack
mov esp, kernel_stack
add esp, INITSTACKSIZE
sub esp, KERNEL_HIGH_VMA

mov edx, _kernel_end
sub edx, KERNEL_HIGH_VMA
push edx
mov edx, _kernel_start
sub edx, KERNEL_HIGH_VMA
push edx

push ebx ; mbi
push eax ; magic

call boot_init_mem

jmp permahalt

.end:

permahalt:
    cli
    hlt
    
gdt_load:
    mov	eax, [esp + 4]
    lgdt	[eax]
    ; code segment 0x8 -> 0x8 * 8 = 64B offset
    jmp	0x8:.reload_cs
.reload_cs:
    mov ax, 0x10
    mov	ds, ax
    mov	es, ax
    mov	fs, ax
    mov	gs, ax
    mov	ss, ax
    ret

load_idt: 
	mov edx, [esp + 4]
	lidt [edx]
	ret