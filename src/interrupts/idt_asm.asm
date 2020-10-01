%macro IRQ 1
global irq%1
extern irq%1_handler
global irq%1_handler
irq%1:
    pusha ; push all reg
    call irq%1_handler
    popa ; pop all reg
    iret
%endmacro

%macro isr_err 1
global isr%1
align 4
isr%1:
    cli ; handling error so turned off interrupts for moment

    ; ERROR CODE HAS BEEN PUSHED TO STACK
    push %1 ; push isr id on stack

    ; save registers eax ebx ecx ect.
    pusha 

    ; push segment registers
    push	ds
    push	es
    push	fs
    push	gs
    
    ; set segment descriptors to kernel data
    mov ax, 0x10 
    mov ds, ax 
    mov es, ax
    mov fs, ax
    mov gs, ax

    extern isr_handler
    call isr_handler

    ; pop segment register
    pop gs
    pop fs
    pop es
    pop ds

    ; pop registers
    popa

    add esp, 8 ; restore stack pointer ;;; err_code and isr id pushed
    
    sti ;enable interrupts

iret
%endmacro

%macro isr_no_err 1
global isr%1
align 4
isr%1:
    cli ; handling error so turned off interrupts for moment
    
    push 0 ; no error 
    push %1 ; push isr id on stack

    ; save registers eax ebx ecx ect.
    pusha 

    ; push segment registers
    push	ds
    push	es
    push	fs
    push	gs
    
    ; set segment descriptors to kernel data
    mov ax, 0x10 
    mov ds, ax 
    mov es, ax
    mov fs, ax
    mov gs, ax

    extern isr_handler
    call isr_handler

    ; pop segment register
    pop gs
    pop fs
    pop es
    pop ds

    ; pop registers
    popa

    add esp, 8 ; restore stack pointer ;;; err_code and isr id pushed
    
    sti ;enable interrupts

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

isr_no_err 0
isr_no_err 1
isr_no_err 2
isr_no_err 3
isr_no_err 4
isr_no_err 5
isr_no_err 6
isr_no_err 7
isr_err 8
isr_no_err 9
isr_err 10
isr_err 11
isr_err 12
isr_err 13
isr_err 14
isr_no_err 15
isr_no_err 16
isr_err 17
isr_no_err 18
isr_no_err 19
isr_no_err 20
isr_no_err 21
isr_no_err 22
isr_no_err 23
isr_no_err 24
isr_no_err 25
isr_no_err 26
isr_no_err 27
isr_no_err 28
isr_no_err 29
isr_err 30
isr_no_err 31

isr_no_err 186
