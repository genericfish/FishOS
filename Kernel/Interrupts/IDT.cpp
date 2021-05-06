#include <FSH/Platform.h>
#include <FSH/Types.h>

#include <Kernel/Interrupts/IDT.h>

#include <string.h>

extern u32 CODE_SEG;
extern u32 DATA_SEG;

struct PACKED IDT_entry {
    u16 base_lo;
    u16 sel;
    u8 zero;
    u8 flags;
    u16 base_hi;
};

struct PACKED IDT_ptr {
    u16 limit;
    u32 base;
};

IDT_entry idt[256];

constexpr void idt_set_gate(u8 num, u32 base, u16 sel, u8 flags) {
    u16 base_lo = base & 0xFFFF;
    u16 base_hi = (base >> 0xF) & 0xFFFF;

    idt[num] = IDT_entry {
        .base_lo = base_lo, .sel = sel, .zero = 0, .flags = flags, .base_hi = base_hi
    };
}

void idt_install() {
    IDT_ptr idtp = { .limit = (sizeof(IDT_entry) * 256) - 1, .base = reinterpret_cast<u32>(&idt) };

    memset(reinterpret_cast<void*>(&idt), 0, idtp.limit + 1);

    __asm__("lidt %0" : : "m"(idtp));
}
