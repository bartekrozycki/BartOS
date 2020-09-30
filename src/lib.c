#include "lib.h"
#include "ints.h"

int memcmp(const void* p1, const void* p2, size_t n)
{
    register const unsigned char *ptr1 = (const unsigned char *) p1, *ptr2 = (const unsigned char *) p2;

    while (n-- > 0)
        if (*ptr1++ != *ptr2++)
            return *(ptr1-1) < *(ptr2-1) ? -1 : 1;

    return 0;
}
void* memcpy(void* dest, const void* src, size_t n)
{
    char *d = dest;
    const char *s = src;
    while (n--)
        *d++ = *s++;
    
    return dest;
}
void* memset(void* ptr, int c, size_t n)
{
    unsigned char *p = ptr;
    while (n-- > 0)
        *p++ = c;
    return p;
}
char* strchr(register const char* str, int c)
{
    do {
        if (*str == c)
            return (char*)c;
    } while (*str++);

    return NULL;
}
int strcmp(const char* s1, const char* s2)
{
    while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}
char* strcpy(char* dest, const char* src)
{
    size_t i;
    for (i = 0; src[i] != '\0'; i++)
        dest[i] = src[i];
    dest[i] = '\0';
    return dest;
}
//                           ///////////////////// TODO MALLOC REQUIRED ====================================================================
char* strdup(const char* str)
{
    // size_t len = strlen (str) + 1;

    // char *result = (char*) malloc (len); 
    
    // if (result == NULL)
    //     return (char*) 0;
    
    // return (char*) memcpy (result, str, len);
    return NULL;
}
size_t strlen(const char* str)
{
    register size_t len = 0;
    register const char *ptr = str;

    while (*ptr++)
        len++;

    return len;
}
