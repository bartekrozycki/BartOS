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
