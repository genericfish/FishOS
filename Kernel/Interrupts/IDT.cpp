#include <FSH/Types.h>

#include <Kernel/Interrupts/Interrupts.h>

#include <string.h>

namespace Interrupts::IDT {
IDT_Entry IDT[256];

void idt_set_gate(u8 num, u64 base, u16 sel, u8 flags)
{
    IDT[num].base_lo = base & 0xFFFF;
    IDT[num].base_mid = (base >> 0x10) & 0xFFFF;
    IDT[num].base_hi = (base >> 0x20) & 0xFFFFFFFF;
    IDT[num].sel = sel;
    IDT[num].flags = flags;
    IDT[num].zero = 0;
}

IDT_Entry const& idt_get_gate(size_t i) { return IDT[i]; }

void setup()
{
    Descriptor desc = {
        .limit = (sizeof(IDT_Entry) * 256) - 1,
        .base = reinterpret_cast<u64>(IDT)
    };

    memset(reinterpret_cast<void*>(IDT), 0, desc.limit + 1);

    asm volatile("lidt %0"
                 :
                 : "m"(desc));
}
}
