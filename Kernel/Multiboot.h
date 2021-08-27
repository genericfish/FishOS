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

template <size_t N>
struct multiboot_type {
    using type = multiboot_tag;
};

#define ADD_MULTIBOOT_TYPE(N, T) \
    template <>                  \
    struct multiboot_type<N> {   \
        using type = T;          \
    }

ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_CMDLINE, multiboot_tag_string);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME, multiboot_tag_string);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_MODULE, multiboot_tag_module);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_BASIC_MEMINFO, multiboot_tag_basic_meminfo);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_BOOTDEV, multiboot_tag_bootdev);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_MMAP, multiboot_tag_mmap);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_VBE, multiboot_tag_vbe);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_FRAMEBUFFER, multiboot_tag_framebuffer);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_ELF_SECTIONS, multiboot_tag_elf_sections);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_APM, multiboot_tag_apm);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_EFI32, multiboot_tag_efi32);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_EFI64, multiboot_tag_efi64);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_SMBIOS, multiboot_tag_smbios);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_ACPI_OLD, multiboot_tag_old_acpi);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_ACPI_NEW, multiboot_tag_new_acpi);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_NETWORK, multiboot_tag_network);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_EFI_MMAP, multiboot_tag_efi_mmap);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_EFI32_IH, multiboot_tag_efi32_ih);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_EFI64_IH, multiboot_tag_efi64_ih);
ADD_MULTIBOOT_TYPE(MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR, multiboot_tag_load_base_addr);

template <size_t N>
FLATTEN multiboot_type<N>::type* get_multiboot_tag(u64 addr)
{
    auto* cur = reinterpret_cast<multiboot_tag*>(addr + 8);

    while (true) {
        if (cur->type == MULTIBOOT_TAG_TYPE_END)
            return nullptr;

        if (cur->type == N)
            return reinterpret_cast<multiboot_type<N>::type*>(cur);

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
