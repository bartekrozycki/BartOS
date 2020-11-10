global _start:function (_start.end - _start)

;;;;;;;;;;;;;;;;;;;;;;;;;; link.ld

extern KERNEL_BOOT_VMA
extern KERNEL_HIGH_VMA

;;;;;;;;;;;;;;;;;;;;;;;;;;

extern init_memory_manager
extern permahalt
extern Main

; Contstants for Multiboot
FLAGS	        equ	11b
MAGIC	        equ	0x1BADB002
CHKSUM	        equ	-(MAGIC + FLAGS)
INITSTACKSIZE	equ	0x8000 ; 1MiB

section .data
    align 0x1000
    kernel_stack            times INITSTACKSIZE         db 0

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

push ebx
push eax

push ebx

call init_memory_manager

cmp eax, 0
jne _start.perm ; if status code != 0 permahalt

call Main

.perm:      ; shithappend
    cli
    hlt

.end: