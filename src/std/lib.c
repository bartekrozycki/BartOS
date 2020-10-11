#include "lib.h"
#include "terminal.h"
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
    return (char *) str;
}
size_t strlen(const char* str)
{
    register size_t len = 0;
    register const char *ptr = str;

    while (*ptr++)
        len++;

    return len;
}

char *itoa (int value, char * str, int base )
{
    if (!str)
        return NULL;

    if (base < 2 || base > 16) //base not supported yet :)
        return NULL;

    u32 uvalue;
    u32 i = 0;
    
    /* 
     * Negative numbers are only supported for decimal.
     * Cast to unsigned to avoid overflow for maximum negative value.  
     *                                                                  */
    
    if ((base == 10) && (value < 0))
    {              
        *(str) = '-';
        uvalue = (unsigned)-value;
        i++;
    }
    else
        uvalue = (unsigned)value;
    
    return utoa(uvalue, str + i, base);
}
char *utoa (unsigned int value, char * str, int base )
{
    const char digits[] = "0123456789ABCDEF";
    int i, j;
    unsigned remainder;
    char c;

    /* Check base is supported. */
    if ((base < 2) || (base > 16))
    { 
        str[0] = '\0';
        return NULL;
    }  

    /* Convert to string. Digits are in reverse order.  */
    i = 0;
    do 
    {
        remainder = value % base;
        str[i++] = digits[remainder];
        value = value / base;
    } while (value != 0);  
    str[i] = '\0'; 

    /* Reverse string.  */
    for (j = 0, i--; j < i; j++, i--)
    {
        c = str[j];
        str[j] = str[i];
        str[i] = c; 
    }       

    return str;
    
}