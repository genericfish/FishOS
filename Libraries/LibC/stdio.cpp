#include <FSH/Types.h>

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifdef __LIBK
#include <Kernel/TTY/TTY.h>
#endif

int puts(char const* str) { return printf("%s\n", str); }

int putchar(int ic) {
#ifdef __LIBK
    auto c = static_cast<char>(ic);
    terminal_write(&c, sizeof(c));
#else
// TODO: stdio and write syscall
#endif

    return ic;
}

static bool print(char const* data, size_t length) {
    auto const* bytes = reinterpret_cast<u8 const*>(data);

    for (auto i = 0uz; i < length; i++)
        if (putchar(bytes[i]) == EOF)
            return false;

    return true;
}

int printf(char const* restrict format, ...) {
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0') {
        auto maxrem = INT_MAX - written;

        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;

            auto amount = 1uz;

            while (format[amount] && format[amount] != '%')
                amount++;

            if (maxrem < amount) // FIXME: EOVERFLOW
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

            if (!maxrem) // FIXME: EOVERFLOW
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

            if (maxrem < len) // FIXME: EOVERFLOW
                return -1;

            if (!print(str, len))
                return -1;

            written += len;

            continue;
        }

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