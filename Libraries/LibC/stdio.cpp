#include <FSH/Types.h>

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __LIBK
#    include <Kernel/TTY/TTY.h>
#endif

int puts(char const* str) { return printf("%s\n", str); }

int putchar(int ic)
{
#ifdef __LIBK
    auto c = static_cast<char>(ic);
    TTY::write(&c, sizeof(c));
#else
// TODO: stdio and write syscall
#endif

    return ic;
}

static bool print(char const* data, size_t length)
{
    auto const* bytes = reinterpret_cast<u8 const*>(data);

    for (auto i = 0UZ; i < length; i++)
        if (putchar(bytes[i]) == EOF)
            return false;

    return true;
}

int printf(char const* restrict format, ...)
{
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0') {
        auto rst = INT_MAX - written;

        if (rst < 0)
            return -1;

        auto maxrem = static_cast<u32>(rst);

        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;

            auto amount = 1UZ;

            while (format[amount] && format[amount] != '%')
                amount++;

            if (maxrem < amount)
                return -1;

            if (!print(format, amount))
                return -1;

            format += amount;
            written += amount;

            continue;
        }

        auto const* format_begun_at = format++;

        if (*format == 'c') {
            format++;
            auto c = static_cast<char>(va_arg(parameters, int));

            if (!maxrem)
                return -1;

            if (!print(format, sizeof(c)))
                return -1;

            written++;

            continue;
        }

        if (*format == 's') {
            format++;
            auto const* str = va_arg(parameters, char const*);
            auto len = strlen(str);

            if (maxrem < len)
                return -1;

            if (!print(str, len))
                return -1;

            written += len;

            continue;
        }

        static char str[sizeof(long) * 8 + 1];
        auto long_mode = false;

        if (format[0] == 'l' && format[1] == 'l') {
            format += 2;
            long_mode = true;
        }

        if (long_mode || *format == 'd' || *format == 'x' || *format == 'b' || *format == 'o') {
            auto d = va_arg(parameters, long);

            if (!maxrem)
                return -1;

            auto radix = 10;

            if (*format == 'x')
                radix = 16;
            else if (*format == 'b')
                radix = 2;
            else if (*format == 'o')
                radix = 8;

            char* dstr = reinterpret_cast<char*>(0);

            if (long_mode)
                dstr = ltoa(d, str, radix);
            else
                dstr = itoa(static_cast<i32>(d), str, radix);

            while (*dstr != '\0')
                if (!print(dstr++, 1))
                    return -1;

            format++;
            continue;
        }

        if (long_mode)
            return -1;

        format = format_begun_at;
        auto len = strlen(format);

        if (maxrem < len) // FIXME: EOVERFLOW
            return -1;

        if (!print(format, len))
            return -1;

        written += len;
        format += len;
    }

    va_end(parameters);
    return written;
}
