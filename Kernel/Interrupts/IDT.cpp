#include <FSH/Platform.h>
#include <FSH/Types.h>

#include <Kernel/Interrupts/Interrupts.h>

#include <stdio.h>
#include <string.h>

extern "C" void load_idt(IDT_Descriptor*);

IDT_Entry idt[256];

void idt_set_gate(u8 num, u64 base, u16 sel, u8 flags)
{
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_mid = (base >> 0x10) & 0xFFFF;
    idt[num].base_hi = (base >> 0x20) & 0xFFFFFFFF;
    idt[num].sel = sel;
    idt[num].flags = flags;
    idt[num].zero = 0;
}

IDT_Entry const& idt_get_gate(size_t i) { return idt[i]; }

void idt_install()
{
    IDT_Descriptor desc = {
        .limit = (sizeof(IDT_Entry) * 256) - 1,
        .base = idt
    };

    memset(reinterpret_cast<void*>(&idt), 0, desc.limit + 1);

    load_idt(&desc);
}
