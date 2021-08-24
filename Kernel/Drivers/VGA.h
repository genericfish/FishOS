#pragma once

#include <FSH/Platform.h>
#include <FSH/Types.h>

namespace Kernel::Drivers::VGA {
enum Color : u8 {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15,
};

static inline u8 vga_entry_color(enum Color fg, enum Color bg)
{
    return bg << 4 | fg;
}

static inline u16 vga_entry(u8 uc, u8 color)
{
    return static_cast<u16>(color) << 8 | static_cast<u16>(uc);
}
}
