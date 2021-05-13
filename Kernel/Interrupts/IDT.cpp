#include <FSH/Platform.h>
#include <FSH/Types.h>

#include <Kernel/Interrupts/Interrupts.h>

#include <string.h>

extern "C" void load_idt(u32);

IDT_entry idt[256];

void idt_set_gate(u8 num, u32 base, u16 sel, u8 flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 0x10) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

IDT_entry const& idt_get_gate(size_t i) { return idt[i]; }

void idt_install() {
    IDT_ptr idtp = { .limit = (sizeof(IDT_entry) * 256) - 1, .base = reinterpret_cast<u32>(&idt) };

    memset(static_cast<void*>(&idt), 0, idtp.limit + 1);

    load_idt(reinterpret_cast<u32>(&idtp));
}
