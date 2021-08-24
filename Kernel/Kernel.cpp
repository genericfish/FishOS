#include <FSH/Format.h>
#include <FSH/Platform.h>
#include <FSH/Types.h>
#include <FSH/Utility.h>

#include <Kernel/Drivers/Serial/Serial.h>
#include <Kernel/Drivers/TTY/TTY.h>
#include <Kernel/Drivers/VGA.h>
#include <Kernel/Interrupts/Interrupts.h>
#include <Kernel/Memory/Address.h>
#include <Kernel/Multiboot.h>
#include <Kernel/StdLib.h>
#include <Kernel/multiboot2.h>

#include <stdio.h>
#include <sys/io.h>

using namespace Kernel;
using namespace Address;
using namespace Drivers;
using namespace Multiboot;

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

    if (multiboot_magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        printf("Non-multiboot2 compliant bootloader. Expected bootloader magic: 0x%llx, got 0x%llx instead.\n", MULTIBOOT2_BOOTLOADER_MAGIC, multiboot_magic);
        return -1;
    }

    if (multiboot_addr & 7) {
        printf("Unaligned multiboot tag pointer: 0x%llx\n", multiboot_addr);
        return -1;
    }

    Interrupts::setup();

    asm("sti");

    printfsh();

    multiboot_addr += g_virtual_base_address;

    auto serial = Serial {};

    auto* bootloader_info_tag = get_multiboot_tag<multiboot_tag_string>(multiboot_addr);

    serial.write("[FishOS] Verified bootloader: ");
    serial.write(reinterpret_cast<char*>(&bootloader_info_tag->string));
    serial.write('\n');

    auto* mmap = get_multiboot_tag<multiboot_tag_mmap>(multiboot_addr);

    for (auto* ptr = mmap->entries; reinterpret_cast<u8*>(ptr) < reinterpret_cast<u8*>(mmap) + mmap->size; ptr = reinterpret_cast<decltype(ptr)>(reinterpret_cast<u8*>(ptr) + mmap->entry_size)) {
        printf("Section: Addr: 0x%llx, Type: 0x%llx, Len: 0x%llx\n", ptr->addr, ptr->type, ptr->len);
    }

    auto* elf = get_multiboot_tag<multiboot_tag_elf_sections>(multiboot_addr);

    for (auto* ptr = reinterpret_cast<Elf64_Shdr*>(elf->sections); reinterpret_cast<u8*>(ptr) < reinterpret_cast<u8*>(elf) + elf->size; ptr = reinterpret_cast<Elf64_Shdr*>(reinterpret_cast<u8*>(ptr) + elf->entsize)) {
        printf("ELF Shdr: Addr: 0x%llx, Type: %llx, Flags: 0x%llx\n", ptr->sh_addr, ptr->sh_type, ptr->sh_flags);
    }

    while (true)
        continue;

    return 0;
}
