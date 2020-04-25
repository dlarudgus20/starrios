#pragma once

#include <stddef.h>
#include <stdarg.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define range(a, x, b) (((x) < (a)) ? (a) : (min(x, b)))

void* memset(void* dest, int ch, size_t count);
void* memcpy(void* restrict dest, const void* restrict src, size_t count);
void* memmove(void* dest, const void* src, size_t count);

int isalpha(int c);
int isdigit(int c);

size_t strlen(const char* str);
size_t strnlen(const char* str, size_t size);
char* strcpy(char* restrict dest, const char* restrict src);
char* strncpy(char* restrict dest, const char* restrict src, size_t count);
char* strcat(char* restrict dest, const char* restrict src);
int strcmp(const char* lhs, const char* rhs);

int vsnprintf(char *buf, size_t size, const char *format, va_list va);
int snprintf(char *buf, size_t size, const char *format, ...);
