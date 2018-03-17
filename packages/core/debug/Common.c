/*  - Common.c
    AUTHOR: Will Fantom
    DESC:   Offers some new data types for readability along with some core
            functions that any module could need.
*/

#include <core/debug/Common.h>

void memcpy(void *dest, const void *src, unsigned int len)
{
    char *d = (char *)dest;
    const char *s = (const char *)src;
    while(len--)
    {
        *d++ =*s++;
    }
    return dest;
}

void memset(void* dest, unsigned char val, unsigned int len)
{
    unsigned char *d = (unsigned char *)dest;
    while(len > 0)
    {
        *d++ = val;
        len--;
    }
}


void empty()
{

}
