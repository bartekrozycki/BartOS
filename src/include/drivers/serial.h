#pragma once

#include "ints.h"

#define PORT 0x3f8   /* COM1 */
 
void init_serial();
char serial_read();

void serial_putc(char a); // put char in stream
void serial_write(char *string, u32 n); // write N chars from givven address
void serial_writestring(char *string); // write string till \0 
void serial_writebase(i32 d, u32 base); // write interger in base (supported 2 - 16)
