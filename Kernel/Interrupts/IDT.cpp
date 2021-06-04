#include <FSH/Platform.h>
#include <FSH/Types.h>

#include <Kernel/Interrupts/Interrupts.h>

#include <string.h>
#include <stdio.h>

extern "C" void load_idt(u64);

IDT_Entry idt[256];

void idt_set_gate(u8 num, u64 base, u16 sel, u8 flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_med = (base >> 0x10) & 0xFFFF;
    idt[num].base_hi = base >> 0x20;
    idt[num].sel = sel;
    idt[num].flags = flags;
    idt[num].zero = 0;
}

IDT_Entry const& idt_get_gate(size_t i) { return idt[i]; }

void idt_install() {
    IDT_ptr idtp = { .limit = (sizeof(IDT_Entry) * 256) - 1, .base = reinterpret_cast<u64>(&idt) };

    memset(static_cast<void*>(&idt), 0, idtp.limit + 1);

    load_idt(reinterpret_cast<u64>(&idtp));
}
