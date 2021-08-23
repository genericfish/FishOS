#include <FSH/Platform.h>
#include <FSH/Types.h>
#include <FSH/Utility.h>

#include <Kernel/Drivers/TTY/TTY.h>
#include <Kernel/Drivers/Serial/Serial.h>
#include <Kernel/Interrupts/Interrupts.h>
#include <Kernel/Memory/Address.h>

#include <Kernel/multiboot2.h>

#include <stdio.h>
#include <sys/io.h>

using namespace Kernel::Address;
using namespace Kernel::Drivers;

extern "C" u64 init_pml4;
extern "C" u64 _kernel_start;
extern "C" u64 _kernel_end;

void printfsh()
{
    printf("      ===================          ====\n");
    printf("    ===                 ===       == ==\n");
    printf("  ===    ==               ====   ==  ==\n");
    printf("  =      ==                  =====   ==\n");
    printf("  ===                     ====  ==   ==\n");
    printf("    ===                 ===       == ==\n");
    printf("      ===================          ====\n");
    printf("            FishOS\n\n");
}

extern "C" int main(u64 multiboot_magic, u64 multiboot_addr)
{
    TTY::clear(VGA::vga_entry_color(VGA::WHITE, VGA::BLUE));

    Interrupts::setup();

    asm("sti");

    printfsh();

    if (multiboot_magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        printf("Non-multiboot2 compliant bootloader. Expected bootloader magic: 0x%llx, got 0x%llx instead.\n", MULTIBOOT2_BOOTLOADER_MAGIC, multiboot_magic);
        return -1;
    }

    if (multiboot_addr & 7) {
        printf("Unaligned multiboot info pointer: 0x%llx\n", multiboot_addr);
        return -1;
    }

    multiboot_addr += 0xFFFFFFFF80000000ULL;

    auto* tag = reinterpret_cast<multiboot_tag*>(multiboot_addr + 8);

    while (tag->type != MULTIBOOT_TAG_TYPE_END) {
        printf("Tag 0x%llx, size 0x%llx\n", tag->type, tag->size);

        if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
            // Print out memory areas
            for (auto* mmap = reinterpret_cast<multiboot_memory_map_t*>(reinterpret_cast<multiboot_tag_mmap*>(tag)->entries);
                 reinterpret_cast<u8*>(mmap) < reinterpret_cast<u8*>(tag + tag->size);
                 mmap = reinterpret_cast<multiboot_memory_map_t*>(reinterpret_cast<u64>(mmap) + reinterpret_cast<multiboot_tag_mmap*>(tag)->size)) {
                printf("Section: 0x%llx, 0x%llx, 0x%llx\n", mmap->addr, mmap->type, mmap->len);
            }
    }

        // TODO: ELF Section Headers

        tag = reinterpret_cast<multiboot_tag*>(reinterpret_cast<u8*>(tag) + ((tag->size + 7) & ~7));
    }

    while (true)
        continue;

    return 0;
}
