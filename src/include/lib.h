#pragma once

#include "terminal.h"

#define ASSERT(str, size) \
if (sizeof(str) != size) {\
   terminal_writestring("(Assert failed) " #str " =/= " #size "."); \
   while(1);\
}

// stddef.h
#define NULL ((void*) 0)
typedef unsigned int size_t;

// stdbool.h
#define true  1
#define false 0

// stdarg.h
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)
typedef __builtin_va_list va_list;

// string.h
int memcmp(const void* p1, const void* p2, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* ptr, int c, size_t n);
char* strchr(const char* str, int c);
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
char* strdup(const char* str);
size_t strlen(const char* str);
char *utoa (int value, char * str, int base);
char *itoa (int value, char * str, int base);


