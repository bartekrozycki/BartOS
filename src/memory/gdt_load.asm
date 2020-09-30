global gdt_load
gdt_load:
    ; load the GDT
    mov	eax, [esp + 4]
    lgdt	[eax]
    ; set segment registers to 0x10
    jmp	0x0008:.reload_cs
    .reload_cs:
        mov	ax, 0x0010
        mov	ds, ax
        mov	es, ax
        mov	fs, ax
        mov	gs, ax
        mov	ss, ax
    ret
