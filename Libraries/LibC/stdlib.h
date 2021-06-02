#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

__attribute__((__noreturn__)) void abort(void);
char* itoa(int, char*, int);

__END_DECLS
