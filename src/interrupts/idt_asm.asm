BITS 32
%macro IRQ 1
global irq%1
extern irq%1_handler
global irq%1_handler
irq%1:
    mov byte [0xb8000], 'a'
    pusha
    call irq%1_handler
    popa
    iret
%endmacro

global load_idt
; Interrupts has been blocked in (Boot.asm):31
load_idt: 
	mov edx, [esp + 4]
	lidt [edx]
	sti
	ret

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15
 