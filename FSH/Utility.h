#pragma once

#include <FSH/Types.h>

template <size_t... I>
struct index_sequence {
    using self_t = index_sequence;
    using value_t = size_t;

    static constexpr size_t size() noexcept { return sizeof...(I); };
};

template <class S1, class S2>
struct concat;

template <size_t... I1, size_t... I2>
struct concat<index_sequence<I1...>, index_sequence<I2...>>
    : index_sequence<I1..., (sizeof...(I1) + I2)...> {
};

template <size_t N>
struct make_index_sequence : concat<typename make_index_sequence<N / 2>::self_t,
                                    typename make_index_sequence<N - N / 2>::self_t> {
};

template <>
struct make_index_sequence<0> : index_sequence<> {
};
template <>
struct make_index_sequence<1> : index_sequence<0> {
};
