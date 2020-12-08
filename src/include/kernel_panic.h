/*
    k e r n e l  p a n i c
*/
#include "paging.h"

#define BOCHS_BREAK __asm__("xchgw %bx, %bx")

#define STR(x) #x
#define STRINGIFY(x) STR(x)
#define FILE_LINE __FILE__ ":" STRINGIFY(__LINE__)
#define kPanic k_panic(FILE_LINE)

_Noreturn static inline void k_panic(char *str)
{
    map(0xb8000, 0xb8000);
    unsigned short *video = (unsigned short*) 0xb8000; //
    char *rip =  "R I P    K E R N E L   i n   ";

    while(*rip != '\0')
        *video++ = (unsigned char) *rip++ | (0x2 << 8);

    while(*str != '\0')
        *video++ = (unsigned char) *str++ | (0x2 << 8);

    __asm__("cli");
    while (1)
            __asm__("hlt");
}
