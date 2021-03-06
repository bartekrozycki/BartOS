#include "print.h"
#include "serial.h"
#include "vga.h"
#include "lib.h"


u32 print(STREAM stream, char *format, ...)
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
            u32 num;
            switch (*(format + ++i))
            {
                case 'c': // char
                    czar = (char) va_arg(va, int);

                    if (stream == TERMINAL)
                        terminal_putc(czar);
                    else if (stream == SERIAL)
                        serial_putc(czar);

                    break;
                case 's': // string
                    str = va_arg(va, char *);

                    if (stream == TERMINAL)
                        terminal_writestring(str);
                    else if (stream == SERIAL)
                        serial_writestring(str);

                    break;
                case 'd': // decmial
                    num = va_arg(va, i32);

                    if (stream == TERMINAL)
                        terminal_write_base(num, 10);
                    else if (stream == SERIAL)
                        serial_writebase(num, 10);

                    break;
                case 'u': // unsigned decmial
                    num = va_arg(va, u32);

                    if (stream == TERMINAL)
                        terminal_write_base(num, 10);
                    else if (stream == SERIAL)
                        serial_writebase(num, 10);

                    break;
                case 'b': // binary
                    num = va_arg(va, i32);

                    if (stream == TERMINAL)
                    {
                        terminal_write_base(num, 2);
                        terminal_putc('b');
                    }
                    else if (stream == SERIAL)
                    {
                        serial_writebase(num, 2);
                        serial_putc('b');
                    }
                    break;
                case 'x': // hexadecmial
                    num = va_arg(va, u32);
                    terminal_write_base(num, 16);
                    break;
                case 'p': // pointer
                    num = va_arg(va, u32);

                    if (stream == TERMINAL)
                    {
                        terminal_writestring("0x");
                        terminal_write_base(num, 16);
                    }
                    else if (stream == SERIAL)
                    {
                        serial_writestring("0x");
                        serial_writebase(num, 16);
                    }
                    break;
                default:
                    if (stream == TERMINAL)
                        terminal_writestring("%");
                    else if (stream == SERIAL)
                        serial_writestring("%");
                    break;
            }
            ++i;
            continue;
        }

        if (stream == TERMINAL)
            terminal_putc(*(format + i));
        else if (stream == SERIAL)
            serial_putc(*(format + i));

        ++i;
    }
    va_end(va);

    return i;
}

u32 print_at(STREAM stream,size_t x, size_t y, char *format, ...)
{
    if (!format) return 0;

    va_list va;
    va_start(va, format);
    u32 i = 0;
    u32 _x = getX();
    u32 _y = getY();
    terminal_pos(x, y);
    while (*(format + i))
    {
        if (*(format + i) == '%')
        {
            char czar;
            char *str;
            u32 num;
            switch (*(format + ++i))
            {
                case 'c': // char
                    czar = (char) va_arg(va, int);

                    if (stream == TERMINAL)
                        terminal_putc(czar);
                    else if (stream == SERIAL)
                        serial_putc(czar);

                    break;
                case 's': // string
                    str = va_arg(va, char *);

                    if (stream == TERMINAL)
                        terminal_writestring(str);
                    else if (stream == SERIAL)
                        serial_writestring(str);

                    break;
                case 'd': // decmial
                    num = va_arg(va, i32);

                    if (stream == TERMINAL)
                        terminal_write_base(num, 10);
                    else if (stream == SERIAL)
                        serial_writebase(num, 10);

                    break;
                case 'u': // unsigned decmial
                    num = va_arg(va, u32);

                    if (stream == TERMINAL)
                        terminal_write_base(num, 10);
                    else if (stream == SERIAL)
                        serial_writebase(num, 10);

                    break;
                case 'b': // binary
                    num = va_arg(va, i32);

                    if (stream == TERMINAL)
                    {
                        terminal_write_base(num, 2);
                        terminal_putc('b');
                    }
                    else if (stream == SERIAL)
                    {
                        serial_writebase(num, 2);
                        serial_putc('b');
                    }
                    break;
                case 'x': // hexadecmial
                    num = va_arg(va, u32);
                    terminal_write_base(num, 16);
                    break;
                case 'p': // pointer
                    num = va_arg(va, u32);

                    if (stream == TERMINAL)
                    {
                        terminal_writestring("0x");
                        terminal_write_base(num, 16);
                    }
                    else if (stream == SERIAL)
                    {
                        serial_writestring("0x");
                        serial_writebase(num, 16);
                    }
                    break;
                default:
                    if (stream == TERMINAL)
                        terminal_writestring("%");
                    else if (stream == SERIAL)
                        serial_writestring("%");
                    break;
            }
            ++i;
            continue;
        }

        if (stream == TERMINAL)
            terminal_putc(*(format + i));
        else if (stream == SERIAL)
            serial_putc(*(format + i));

        ++i;
    }
    va_end(va);

    terminal_pos(_x, _y);
    return i;
}
