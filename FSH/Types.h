#pragma once

// FIXME: Remove when Intellisense supports GNU++23 (g++11).
#ifndef __INTELLISENSE__
#ifdef __linux__
#error "[ERROR] Kernel: Compile using the cross-compiler."
#endif

#ifndef __x86_64__
#error "[ERROR] Kernel: Compile using x86_64-elf cross-compiler."
#endif
#else
/*
This is invalid C++, user-defined literals must be prefixed with an underscore.
Intellisense does not highlight this as an error, which is useful as the "UZ"
literal suffix is defined in GNU++23 for size_t.
*/
constexpr size_t operator"" UZ(uint64_t);
#endif

#ifdef restrict
#undef restrict
#endif
#define restrict __restrict

using u8 = __UINT8_TYPE__;
using u16 = __UINT16_TYPE__;
using u32 = __UINT32_TYPE__;
using u64 = __UINT64_TYPE__;

using i8 = __UINT8_TYPE__;
using i16 = __UINT16_TYPE__;
using i32 = __UINT32_TYPE__;
using i64 = __UINT64_TYPE__;

#ifdef __LIBK
using size_t = __SIZE_TYPE__;

using uint8_t = u8;
using uint16_t = u16;
using uint32_t = u32;
using uint64_t = u64;

using int8_t = i8;
using int16_t = i16;
using int32_t = i32;
using int64_t = i64;
#else

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#ifdef __ptrdiff_t
using ptrdiff_t = __PTRDIFF_TYPE__;
#endif

#endif

constexpr u64 KiB = 1ULL << 10;
constexpr u64 MiB = 1ULL << 20;
constexpr u64 GiB = 1ULL << 30;
constexpr u64 TiB = 1ULL << 40;
