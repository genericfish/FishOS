#include <FSH/Platform.h>
#include <FSH/Types.h>

#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__)) void abort(void)
{
#ifdef __LIBK
    printf("[Kernel] PANIC: abort()\n");
#else
    printf("abort()\n");
#endif

    while (1) {
    };
    __builtin_unreachable();
}

char* _ntoa(long value, char* str, bool narrow, int maxlen, int base)
{
    static char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    auto* ret = &str[maxlen - 1];
    auto neg = value < 0 && base == 10;
    auto num = static_cast<u64>(value);

    *ret = '\0';

    if (neg) {
        num ^= -1;
        num++;
    } else if (base <= 1 || base > 36) {
        return ret;
    }

    if (narrow)
        num &= static_cast<u32>(-1);

    do {
        *--ret = digits[num % base];
        num /= base;
    } while (num != 0);

    if (neg)
        *--ret = '-';

    return ret;
}

FLATTEN char* itoa(int value, char* str, int base)
{
    return _ntoa(value, str, true, sizeof(int) * 8 + 1, base);
}

FLATTEN char* ltoa(long value, char* str, int base)
{
    return _ntoa(value, str, false, sizeof(long) * 8 + 1, base);
}
