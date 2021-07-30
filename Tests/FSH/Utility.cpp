#include <FSH/Utility.h>

bool test_index_sequence()
{
    constexpr size_t array_size = 128;

    char array[array_size];

    for (auto i = 0; i < array_size; i++)
        array[i] = 0;

    [&]<size_t... I>(index_sequence<I...>)
    {
        ((array[I] = 29), ...);
    }
    (make_index_sequence<array_size> {});

    for (auto i = 0; i < array_size; i++)
        if (array[i] != 29)
            return false;

    return true;
}
