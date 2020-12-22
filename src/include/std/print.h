#pragma once

#include "ints.h"

typedef enum STREAM{
    TERMINAL = 0,
    SERIAL = 1
} STREAM;

u32 print(STREAM stream, char *format, ...);
u32 print_at(STREAM stream,size_t x, size_t y, char *format, ...);