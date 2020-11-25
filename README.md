# BartOS

System require at least 256MiB, heap have fixed 128MiB size :)
VGA memory curently mapped at 0xF000_0000 (=> 0xB8000)

## Virtual Map
### Kernel
>AT 0xE000_0000 
### Heap 
>AT 0xE800_0000 
### Kernel Structs
>AT 0xF800_0000\
0x1000 Paging Directory\
0x400000 Paging Entrys\
0x?????? Physical Bitmap\
0x?????? Physical Memory Address Stack

