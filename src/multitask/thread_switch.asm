global switch_to_thread

struc TCB
    .ESP:    resb   4
    .CR3:    resb   4
    .NEXT:   resb   4
    .STATUS: resb   4
endstruc

;C declaration:
;   void switch_to_task(thread_control_block *next_thread);
;
;WARNING: Caller is expected to disable IRQs before calling, and enable IRQs again after function returns
;
;Source https://wiki.osdev.org/Brendan%27s_Multi-tasking_Tutorial
switch_to_thread:
    ;Save previous task's state

    ;Notes:
    ;  For cdecl; EAX, ECX, and EDX are already saved by the caller and don't need to be saved again
    ;  EIP is already saved on the stack by the caller's "CALL" instruction
    ;  The task isn't able to change CR3 so it doesn't need to be saved
    ;  Segment registers are constants (while running kernel code) so they don't need to be saved
    push ebx
    push esi
    push edi
    push ebp

    mov edi, [ esp + ( 4 + 1 ) * 4 ]
    mov edi, [ edi ]
    mov [edi + TCB.ESP], esp

    ;Load next task's state

    mov esi, [esp + ( 4 + 2 ) * 4]
    mov edi, [esp + ( 4 + 1 ) * 4]
    mov [edi], esi

    mov esp, [esi + TCB.ESP]
    mov eax, [esi + TCB.CR3]
    mov ebx, cr3

    cmp eax, ebx
    je .doneVAS
    mov cr3, eax
.doneVAS:
    pop ebp
    pop edi
    pop esi
    pop ebx

    ret