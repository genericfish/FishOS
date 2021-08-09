#include <FSH/Platform.h>
#include <FSH/Types.h>
#include <FSH/Utility.h>

#include <Kernel/Interrupts/Interrupts.h>
#include <Kernel/TTY/TTY.h>

#include <Kernel/Memory/Address.h>

#include <Kernel/multiboot.h>

#include <stdio.h>
#include <sys/io.h>

using namespace Address;
using namespace Memory;

u64 ticks = 0;

extern "C" int main(u64 multiboot_magic, u64 multiboot_header_ptr)
{
    TTY::clear(VGA::vga_entry_color(VGA::WHITE, VGA::BLUE));

    if (multiboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("Non-multiboot compliant bootloader. (%llx, %llx)\n", multiboot_magic, multiboot_header_ptr);
        return -1;
    }

    auto const& header = *reinterpret_cast<multiboot_info_t*>(multiboot_header_ptr + g_virtual_base_address);

    if (!(header.flags & (1 << 6))) {
        printf("No multiboot mmap provided.\n");
        return -1;
    }

    Interrupts::setup();

    asm("sti");

    printf("      ===================          ====\n");
    printf("    ===                 ===       == ==\n");
    printf("  ===    ==               ====   ==  ==\n");
    printf("  =      ==                  =====   ==\n");
    printf("  ===                     ====  ==   ==\n");
    printf("    ===                 ===       == ==\n");
    printf("      ===================          ====\n");
    printf("            f i s h\n");

    while (true)
        continue;

    return 0;
}
