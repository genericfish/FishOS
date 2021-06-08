#include <FSH/Types.h>

#include <string.h>

int memcmp(void const* s1, void const* s2, size_t n)
{
    auto const* a = reinterpret_cast<u8 const*>(s1);
    auto const* b = reinterpret_cast<u8 const*>(s2);

    for (auto i = 0UZ; i < n; i++) {
        if (a[i] < b[i])
            return -1;
        else if (b[i] < a[i])
            return 1;
    }

    return 0;
}

void* memcpy(void* __restrict dest, void const* __restrict src, size_t n)
{
    auto* d = reinterpret_cast<u8*>(dest);
    auto const* s = reinterpret_cast<u8 const*>(src);

    for (auto i = 0UZ; i < n; i++)
        d[i] = s[i];

    return dest;
}

void* memmove(void* dest, void const* src, size_t n)
{
    auto* d = reinterpret_cast<u8*>(dest);
    auto const* s = reinterpret_cast<u8 const*>(src);

    if (dest == src)
        return dest;

    if (dest < src) {
        for (auto i = 0UZ; i < n; i++)
            d[i] = s[i];
    } else {
        for (auto i = n; i != 0; i--)
            d[i - 1] = s[i - 1];
    }

    return dest;
}

void* memset(void* s, int c, size_t n)
{
    auto* buf = reinterpret_cast<u8*>(s);
    c &= 0xFF;

    for (auto i = 0UZ; i < n; i++)
        buf[i] = static_cast<u8>(c);

    return buf;
}

size_t strlen(char const* s)
{
    auto len = 0UZ;

    while (s[len])
        len++;

    return len;
}
