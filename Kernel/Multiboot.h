#pragma once

#include <FSH/Platform.h>
#include <FSH/Types.h>
#include <FSH/Utility.h>

#include <Kernel/multiboot2.h>

namespace Kernel::Multiboot {

using namespace FSH;

template <typename T>
consteval size_t get_multiboot_flag_from_type()
{
    if constexpr (is_same<T, multiboot_tag_elf_sections>::value)
        return MULTIBOOT_TAG_TYPE_ELF_SECTIONS;

    if constexpr (is_same<T, multiboot_tag_mmap>::value)
        return MULTIBOOT_TAG_TYPE_MMAP;

    if constexpr (is_same<T, multiboot_tag_string>::value)
        return MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME;

    return MULTIBOOT_TAG_TYPE_END;
}

template <typename T>
FLATTEN T* get_multiboot_tag(u64 addr)
{
    auto* cur = reinterpret_cast<multiboot_tag*>(addr + 8);
    auto const flag = get_multiboot_flag_from_type<T>();

    while (true) {
        if (cur->type == MULTIBOOT_TAG_TYPE_END)
            return nullptr;

        if (cur->type == flag)
            return reinterpret_cast<T*>(cur);

        cur = reinterpret_cast<multiboot_tag*>(reinterpret_cast<u8*>(cur) + ((cur->size + 7) & ~7));
    }
}

// TODO: Move this ELF stuff somewhere else.
struct Elf64_Shdr {
    u32 sh_name;
    u32 sh_type;
    u64 sh_flags;
    u64 sh_addr;
    u64 sh_offset;
    u64 sh_size;
    u32 sh_link;
    u32 sh_info;
    u64 sh_addralign;
    u64 sh_entsize;
};

}
