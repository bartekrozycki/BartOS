;%1 function irq suffix
;%2 gate number

%macro IRQ 2
global irq%1
irq%1:
    push 0
    push %2
    pusha ; push all reg
    
    ; push segment registers
    push	ds
    push	es
    push	fs
    push	gs

    call irq_handler

    ; pop segment register
    pop gs
    pop fs
    pop es
    pop ds

    popa ; pop all reg
    add esp, 8
    iret
%endmacro

%macro isr_err 1
global isr%1
align 4
isr%1:
    cli ; handling error so turned off interrupts for moment
    ; ERROR CODE HAS BEEN PUSHED TO STACK
    push %1 ; push isr id on stack
    jmp __isr__
%endmacro

%macro isr_no_err 1
global isr%1
align 4
isr%1:
    cli ; handling error so turned off interrupts for moment
    push 0 ; no error 
    push %1 ; push isr id on stack

    jmp __isr__
%endmacro

extern irq_handler

IRQ 0, 32
    IRQ 1, 33
    IRQ 2, 34
    IRQ 3, 35
    IRQ 4, 36
    IRQ 5, 37
    IRQ 6, 38
    IRQ 7, 39
    IRQ 8, 40
    IRQ 9, 41
    IRQ 10, 42
    IRQ 11, 43
    IRQ 12, 44
    IRQ 13, 45
    IRQ 14, 46
    IRQ 15, 47

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


__isr__:
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
