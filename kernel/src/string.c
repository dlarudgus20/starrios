#include "string.h"

void* memset(void* dest, int ch, size_t count)
{
    char* d = (char*)dest;
    char* e = d + count;
    while (d < e)
    {
        *d++ = ch;
    }
    return dest;
}

void* memcpy(void* restrict dest, const void* restrict src, size_t count)
{
    const char* s = (const char*)src;
    char* d = (char*)dest;
    char* e = d + count;
    while (d < e)
    {
        *d++ = *s++;
    }
    return dest;
}

void* memmove(void* dest, const void* src, size_t count)
{
    const char* s = (const char*)src;
    char* d = (char*)dest;
    if (d <= s)
    {
        char* e = d + count;
        while (d < e)
        {
            *d++ = *s++;
        }
    }
    else
    {
        char* e = d;
        d += count - 1;
        s += count - 1;
        while (d >= e)
        {
            *d-- = *s--;
        }
    }
    return dest;
}

int isalpha(int c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
int isdigit(int c)
{
    return c >= '0' && c <= '9';
}

size_t strlen(const char* str)
{
    const char *p;
    for (p = str; *p != '\0'; p++) { }
    return (size_t)(p - str);
}
size_t strnlen(const char* str, size_t size)
{
    size_t i = 0;
    while (*str++ != 0 && size-- > 0) i++;
    return i;
}

char* strcpy(char* restrict dest, const char* restrict src)
{
    do
    {
        *dest++ = *src;
    } while (*src++ != '\0');
    return dest;
}
char* strncpy(char* restrict dest, const char* restrict src, size_t count)
{
    for (; count != 0; count--)
    {
        *dest++ = *src;
        if (*src++ != '\0')
            break;
    }
    return dest;
}

char* strcat(char* restrict dest, const char* restrict src)
{
    while (*dest++ != '\0') dest++;

    do
    {
        *dest++ = *src++;
    } while (*src != '\0');

    return dest;
}

int strcmp(const char* lhs, const char* rhs)
{
    int ret;

    while (1)
    {
        if ((ret = *lhs - *rhs) != 0)
            return ret;
        if (*lhs == '\0')
            return 0;

        lhs++; rhs++;
    }
}
