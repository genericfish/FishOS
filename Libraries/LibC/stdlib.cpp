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

char* itoa(int val, char* str, int base) {
    static char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static constexpr int maxlen = sizeof(int) * 8 + 1;

    auto* ret = &str[maxlen - 1];

    *ret = '\0';

    if (base <= 1 || base > 36)
        return ret;

    do {
        *--ret = digits[val % base];
        val /= base;
    } while (val != 0);

    return ret;
}
