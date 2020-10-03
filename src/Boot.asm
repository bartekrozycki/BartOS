BITS 32

; Contstants for Multiboot
FLAGS	equ	11b
MAGIC	equ	0x1BADB002
CHKSUM	equ	-(MAGIC + FLAGS)

section .multiboot
align 4
	dd	MAGIC
	dd	FLAGS
	dd	CHKSUM

;==================================

section .bss
global stack_top
align 16
resb	32768 ; 32KiB
stack_top:

;==================================

section .text

global _start:function (_start.end - _start)
_start:

	cli					; BLOCK INTERRUPTS
	mov esp, stack_top

	; multiboot header pointer
	push	ebx
	; multiboot header magic
	push	eax

	; Jump to C code
	extern Main
	call Main

	cli
.hang:	
	hlt
	jmp .hang
.end: