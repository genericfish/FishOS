#include <FSH/Platform.h>
#include <FSH/Types.h>

#include <Kernel/Arch/i686/VGA.h>
#include <Kernel/StdLib.h>
#include <Kernel/TTY/TTY.h>

static size_t constexpr VGA_WIDTH = 80;
static size_t constexpr VGA_HEIGHT = 25;
static u16* const VGA_MEMORY = reinterpret_cast<u16*>(0xC03FF000);

size_t terminal_row;
size_t terminal_column;

u8 terminal_color;
u16* terminal_buffer;

void inline terminal_setcolor(u8 color) { terminal_color = color; }

void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;

    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;

    for (auto y = 0uz; y < VGA_HEIGHT; y++) {
        for (auto x = 0uz; x < VGA_WIDTH; x++) {
            auto const index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void inline terminal_putentryat(char c, u8 color, size_t x, size_t y) {
    auto const index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll() {
    for (auto i = 0uz; i < VGA_HEIGHT; i++)
        for (auto j = 0uz; j < VGA_WIDTH; j++)
            terminal_buffer[i * VGA_WIDTH + j] = terminal_buffer[(i + 1) * VGA_WIDTH + j];
}

void terminal_clear_line() {
    for (auto i = 0uz; i < VGA_WIDTH - 1; i++)
        terminal_putentryat(' ', terminal_color, i, VGA_HEIGHT - 1);
}

void terminal_putchar(char c) {
    if (c != '\n')
        terminal_putentryat(static_cast<u8>(c), terminal_color, terminal_column, terminal_row);

    if (c == '\n' || ++terminal_column == VGA_WIDTH) {
        terminal_column = 0;

        if (++terminal_row == VGA_HEIGHT) {
            terminal_scroll();
            terminal_clear_line();

            terminal_row = VGA_HEIGHT - 1;
        }
    }
}

void terminal_write(char const* data, size_t size) {
    for (auto i = 0uz; i < size; i++)
        terminal_putchar(data[i]);
}

FLATTEN void terminal_writestring(char const* data) { terminal_write(data, strlen(data)); }
