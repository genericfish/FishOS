#pragma once

#include <FSH/Types.h>

static inline void outb(u16 port, u8 val) {
    asm ("outb %0, %1" : : "a"(val), "Nd"(port));
}
