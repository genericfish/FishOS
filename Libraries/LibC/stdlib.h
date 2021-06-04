#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

__attribute__((__noreturn__)) void abort(void);
char* itoa(int value, char* str, int base);
char* ltoa(long value, char* str, int base);

__END_DECLS
