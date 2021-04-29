#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__)) void abort(void) {
#ifdef __LIBK
    printf("[Kernel] PANIC: abort()\n");
#else
    printf("abort()\n");
#endif

    while (1) {
    };
    __builtin_unreachable();
}
