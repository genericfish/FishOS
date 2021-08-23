#pragma once

#include <Kernel/Arch/x86_64/VGA.h>

namespace Kernel::Drivers::TTY {

void clear(VGA::Color bg, VGA::Color fg);
void clear(u8 color);
void putchar(char c);
void scroll();
void clear_line();
void write(char const* data, size_t n);
void writestring(char const* str);

}
