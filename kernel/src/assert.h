#pragma once

#ifndef NDEBUG

void assert_failure(const char* str, const char* file, const char* func, unsigned line);
#define assert(exp) ((void)((exp) || (assert_failure(#exp, __FILE__, __func__, __LINE__), 1)))

#else

#define assert(exp) ((void)0)

#endif
