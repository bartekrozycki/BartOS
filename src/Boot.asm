global _start:function (_start.end - _start)

global page_directory
global page_entry_kernel
global page_entry_low_memory
global page_entry

;;;;;;;;;;;;;;;;;;;;;;;;;; link.ld

extern KERNEL_BOOT_VMA
extern KERNEL_HIGH_VMA
extern _kernel_end
extern _kernel_start

;;;;;;;;;;;;;;;;;;;;;;;;;;

extern init_boot_pagging
extern permahalt
extern Main

; Contstants for Multiboot
FLAGS	        equ	11b
MAGIC	        equ	0x1BADB002
CHKSUM	        equ	-(MAGIC + FLAGS)
INITSTACKSIZE	equ	0x8000 ; 1MiB

section .data
    align 0x1000
    page_directory          times 0x1000                db 0
    page_entry              times 0x1000 times 0x3FE    db 0
    page_entry_low_memory   times 0x1000                db 0
    page_entry_kernel       times 0x1000                db 0
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

call init_boot_pagging 

push ebx
push eax

call Main

jmp permahalt

.end: