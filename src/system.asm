global int_wait_forever
int_wait_forever:
    sti
.loop:
    hlt
jmp	.loop

extern print
global kernel_panic
kernel_panic:
    mov eax, [esp + 4] ; STREAM
    mov ebx, [esp + 8] ; error text pointer
    push ebx
    push eax
    call print
    jmp permahalt

global permahalt 
permahalt:      ; shithappend
    cli
    hlt


global out
out:
    mov	dx, [esp + 4]
    mov	al, [esp + 8]
    out	dx, al
ret


global in
in:
    mov	dx, [esp + 4]
    in	al, dx
ret
