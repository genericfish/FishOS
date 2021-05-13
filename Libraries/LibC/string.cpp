#include <FSH/Types.h>

#include <string.h>

int memcmp(void const* aptr, void const* bptr, size_t size) {
    auto const* a = reinterpret_cast<u8 const*>(aptr);
    auto const* b = reinterpret_cast<u8 const*>(bptr);

    for (auto i = 0uz; i < size; i++) {
        if (a[i] < b[i])
            return -1;
        else if (b[i] < a[i])
            return 1;
    }

    return 0;
}

void* memcpy(void* restrict dstptr, void const* restrict srcptr, size_t size) {
    auto* dst = reinterpret_cast<u8*>(dstptr);
    auto const* src = reinterpret_cast<u8 const*>(srcptr);

    for (auto i = 0uz; i < size; i++)
        dst[i] = src[i];

    return dstptr;
}

void* memmove(void* dstptr, void const* srcptr, size_t size) {
    auto* dst = reinterpret_cast<u8*>(dstptr);
    auto const* src = reinterpret_cast<u8 const*>(srcptr);

    if (dst < src) {
        for (auto i = 0uz; i < size; i++)
            dst[i] = src[i];
    } else {
        for (auto i = size; i != 0; i--)
            dst[i - 1] = src[i - 1];
    }

    return dstptr;
}

void* memset(void* bufptr, int value, size_t size) {
    auto* buf = reinterpret_cast<u8*>(bufptr);
    value &= 0xFF;

    for (auto i = 0uz; i < size; i++)
        buf[i] = static_cast<u8>(value);

    return buf;
}

size_t strlen(char const* str) {
    auto len = 0uz;

    while (str[len])
        len++;

    return len;
}
