#pragma once

#include <FSH/Types.h>

#include <sys/cdefs.h>

__BEGIN_DECLS

static inline void outb(u16 port, u8 val) { asm("outb %0, %1" : : "a"(val), "Nd"(port)); }

static inline u8 inb(u16 port) {
    auto ret = u8 {};

    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));

    return ret;
}

__END_DECLS
