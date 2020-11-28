#include "serial.h"
#include "lib.h"
#include "system.h"
#include "ints.h"

u8 serial_initalized = 0;

void init_serial() {
    out(PORT + 1, 0x00);    // Disable all interrupts
    out(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    out(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    out(PORT + 1, 0x00);    //                  (hi byte)
    out(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    out(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    out(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set

    serial_initalized = 1;
    serial_writestring("(info) Serial initalized\n");
}

int serial_received() {
    return in(PORT + 5) & 1;
}
 
char serial_read() {
    while (serial_received() == 0);
 
    return in(PORT);
}

int is_transmit_empty() {
    return in(PORT + 5) & 0x20;
}
 
void serial_putc(char c) {
    if (serial_initalized == 0) 
        return;

    while (is_transmit_empty() == 0);
 
    out(PORT, c);
}
void serial_write(char *string, u32 n)
{
    if (!string) return;
    for (u32 i = 0; i < n; i++)
        serial_putc(*(string + i++));
}
void serial_writestring(char *string)
{
    if (!string) return;
    for (u32 i = 0; *(string + i); i++)
        serial_putc(*(string + i));
}
void serial_writebase(i32 d, u32 base)
{
    if (base < 2 || base > 16) return;
    char i_dec[32];
    char *x = itoa(d, i_dec, base);
    serial_writestring(x);
}