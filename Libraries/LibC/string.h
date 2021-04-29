#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int memcmp(void const*, void const*, size_t);
void* memcpy(void* __restrict, void const* __restruct, size_t);
void* memmove(void*, void const*, size_t);
void* memset(void*, int, size_t);
size_t strlen(char const*);

__END_DECLS
