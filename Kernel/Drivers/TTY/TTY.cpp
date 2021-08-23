#include <FSH/Platform.h>
#include <FSH/Types.h>

#include <Kernel/Arch/x86_64/VGA.h>
#include <Kernel/Drivers/TTY/TTY.h>
#include <Kernel/Memory/Address.h>
#include <Kernel/StdLib.h>

namespace Kernel::Drivers::TTY {
static u16 constexpr VGA_WIDTH = 80;
static u16 constexpr VGA_HEIGHT = 25;
static u32 constexpr VGA_BUFFER_SIZE = VGA_HEIGHT * VGA_WIDTH;
static u16* VGA_BUFFER = reinterpret_cast<u16*>(0xB8000ULL + g_virtual_base_address);

static u16 g_pos[2] = { 0 };
static u8 g_color = 0;

void clear(VGA::Color bg, VGA::Color fg)
{
    clear(VGA::vga_entry_color(bg, fg));
}

void clear(u8 color)
{
    for (auto i = 0UL; i < VGA_BUFFER_SIZE; i++)
        VGA_BUFFER[i] = VGA::vga_entry(' ', color);

    g_color = color;
    g_pos[0] = g_pos[1] = 0;
}

inline void putentryat(char c, u8 color, size_t x, size_t y)
{
    auto const index = y * VGA_WIDTH + x;
    VGA_BUFFER[index] = VGA::vga_entry(c, color);
}

FLATTEN void scroll()
{
    for (auto i = 0UL; i < (VGA_BUFFER_SIZE - VGA_WIDTH); i++)
        VGA_BUFFER[i] = VGA_BUFFER[i + VGA_WIDTH];

    clear_line();
}

void clear_line()
{
    for (auto i = 0UZ; i < VGA_WIDTH - 1; i++)
        putentryat(' ', g_color, i, VGA_HEIGHT - 1);
}

void putchar(char c)
{
    if (c != '\n')
        putentryat(static_cast<u8>(c), g_color, g_pos[0], g_pos[1]);

    if (c == '\n' || ++g_pos[0] == VGA_WIDTH) {
        g_pos[0] = 0;

        if (++g_pos[1] == VGA_HEIGHT) {
            scroll();

            g_pos[1] = VGA_HEIGHT - 1;
        }
    }
}

FLATTEN void write(char const* data, size_t n)
{
    for (auto i = 0UZ; i < n; i++)
        putchar(data[i]);
}

FLATTEN void writestring(char const* str) { write(str, strlen(str)); }

}
