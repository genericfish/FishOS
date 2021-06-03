#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int memcmp(void const* s1, void const* s2, size_t n);
void* memcpy(void* __restrict__ dest, void const* __restrict src, size_t n);
void* memmove(void* dest, void const* src, size_t n);
void* memset(void* s, int c, size_t n);
size_t strlen(char const* s);

__END_DECLS
