/*
    k e r n e l  p a n i c
*/

#define BOCHS_BREAK __asm__("xchgw %bx, %bx")

#define STR(x) #x
#define STRINGIFY(x) STR(x)
#define FILE_LINE __FILE__ ":" STRINGIFY(__LINE__)
#define kPanic k_panic(FILE_LINE)

extern void permahalt(void); // TODO make it in high kernel beacuse now system gonna crasshhhh
static inline void k_panic(char *str)
{
    unsigned short *video = (unsigned short*) 0xb8000;
    char *rip =  "R I P    K E R N E L   i n   ";

    while(*rip != '\0')
        *video++ = (unsigned char) *rip++ | (0x2 << 8);

    while(*str != '\0')
        *video++ = (unsigned char) *str++ | (0x2 << 8);

    permahalt();
}
