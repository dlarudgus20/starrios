#include "assert.h"
#include "terminal.h"
#include "string.h"

#ifndef NDEBUG

void assert_failure(const char* str, const char* file, const char* func, unsigned line)
{
    term_panic(str);
    //size_t len = strlen(str);
    //size_t filelen = strlen(file);
    //size_t funclen = strlen(func);

    //{
    //    char msg[len + filelen + funclen + 10/*linelen*/ + 5 + 1];
    //    snprintf(msg, sizeof(msg), "[%s:%s:%d] %s", file, func, line, str);
    //    term_panic(msg);
    //}
}

#endif
