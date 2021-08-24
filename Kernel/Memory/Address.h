#pragma once

#include <FSH/Platform.h>
#include <FSH/Types.h>

const u64 g_virtual_base_address = 0xFFFFFFFF80000000ULL;

namespace Kernel::Address {
using PhysicalAddress = u64;

class VirtualAddress {
public:
    explicit VirtualAddress(u64 address)
        : m_address(address)
    {
        if (((m_address >> 47) ^ 0x1FFFF) != 0)
            m_address = sign_extend_address();
    }

    explicit VirtualAddress(void const* pointer)
    {
        m_address = reinterpret_cast<u64>(pointer);
    }

    ~VirtualAddress()
    {
    }

    template <typename PointerType>
    ALWAYS_INLINE PointerType* to_pointer() const
    {
        return reinterpret_cast<PointerType*>(m_address);
    }

    ALWAYS_INLINE u64 as_u64() const
    {
        return m_address;
    }

    ALWAYS_INLINE VirtualAddress operator+(VirtualAddress const& other) const
    {
        return VirtualAddress(m_address + other.m_address);
    }

    ALWAYS_INLINE VirtualAddress operator+(u64 const& other) const
    {
        return VirtualAddress(m_address + other);
    }

    ALWAYS_INLINE VirtualAddress operator-(VirtualAddress const& other) const
    {
        return VirtualAddress(m_address - other.m_address);
    }

    ALWAYS_INLINE VirtualAddress operator-(u64 const& other) const
    {
        return VirtualAddress(m_address - other);
    }

    ALWAYS_INLINE VirtualAddress& operator+=(VirtualAddress const& other)
    {
        m_address += other.m_address;
        return *this;
    }

    ALWAYS_INLINE VirtualAddress& operator+=(u64 const& other)
    {
        m_address += other;
        return *this;
    }

private:
    ALWAYS_INLINE u64 sign_extend_address() const
    {
        return static_cast<u64>(static_cast<i64>(m_address << 16) >> 16);
    }

    u64 m_address;
};
}
