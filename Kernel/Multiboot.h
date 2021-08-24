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
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_CMDLINE> {
    using type = multiboot_tag_string;
};

template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME> {
    using type = multiboot_tag_string;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_MODULE> {
    using type = multiboot_tag_module;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_BASIC_MEMINFO> {
    using type = multiboot_tag_basic_meminfo;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_BOOTDEV> {
    using type = multiboot_tag_bootdev;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_MMAP> {
    using type = multiboot_tag_mmap;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_VBE> {
    using type = multiboot_tag_vbe;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_FRAMEBUFFER> {
    using type = multiboot_tag_framebuffer;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_ELF_SECTIONS> {
    using type = multiboot_tag_elf_sections;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_APM> {
    using type = multiboot_tag_apm;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_EFI32> {
    using type = multiboot_tag_efi32;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_EFI64> {
    using type = multiboot_tag_efi64;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_SMBIOS> {
    using type = multiboot_tag_smbios;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_ACPI_OLD> {
    using type = multiboot_tag_old_acpi;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_ACPI_NEW> {
    using type = multiboot_tag_new_acpi;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_NETWORK> {
    using type = multiboot_tag_network;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_EFI_MMAP> {
    using type = multiboot_tag_efi_mmap;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_EFI32_IH> {
    using type = multiboot_tag_efi32_ih;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_EFI64_IH> {
    using type = multiboot_tag_efi64_ih;
};
template <>
struct multiboot_type<MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR> {
    using type = multiboot_tag_load_base_addr;
};

template<size_t N>
FLATTEN multiboot_type<N>::type* get_multiboot_tag(u64 addr) {
    auto* cur = reinterpret_cast<multiboot_tag*>(addr + 8);

    while(true) {
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
