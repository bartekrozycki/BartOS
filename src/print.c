#include "print.h"
#include "lib.h"

u32 printf(char *format, ...)
{
    if (!format) return 0;
    
    va_list va;
    va_start(va, format);
    u32 i = 0;

    while (*(format + i))
    {
        if (*(format + i) == '%')
        {
            char czar;
            char *str;
            i32 num;
            switch (*(format + ++i))
            {
                case 'c': // char
                    czar = (char) va_arg(va, int);
                    terminal_putchar(czar);
                    break;
                case 's': // string
                    str = va_arg(va, char *);
                    terminal_writestring(str);
                    break;
                case 'd': // decmial
                    num = va_arg(va, i32);
                    terminal_write_base(num, 10);
                    break;
                case 'u': // unsigned decmial
                    num = va_arg(va, u32);
                    terminal_write_base(num, 10);
                    break;
                case 'b': // binary
                    num = va_arg(va, i32);
                    terminal_write_base(num, 2);
                    terminal_putchar('b');
                    break;
                case 'x': // hexadecmial
                    num = va_arg(va, i32);
                    terminal_write_base(num, 16);
                    break;
                case 'p': // pointer
                    num = va_arg(va, i32);
                    terminal_writestring("0x");
                    terminal_write_base(num, 16);
                    break;
            }
            ++i;
            continue;
        }
        terminal_putchar(*(format + i));
        ++i;
    }
    return i;
}