global gdt_load
gdt_load:
    ; load the GDT
    ;xchg bx, bx
    mov	eax, [esp + 4]
    lgdt	[eax]
    ; set segment registers to 0x10
    mov ax, 0x10
    mov	ds, ax
    mov	es, ax
    mov	fs, ax
    mov	gs, ax
    mov	ss, ax
    jmp	0x8:.reload_cs
    .reload_cs:

    ret
