#include <FSH/Platform.h>
#include <FSH/Types.h>

#include <Kernel/Interrupts/Interrupts.h>

struct PACKED gdt_entry {
    u16 limit_low, base_low;
    u8 base_middle, access, granularity, base_high;
};

struct PACKED gdt_ptr {
    u16 limit;
    u32 base;
};

extern "C" void load_gdt(u32);

gdt_entry gdt[3];

void gdt_set_gate(i32 num, u32 base, u32 limit, u8 access, u8 gran) {
    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].access = access;
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[num].base_high = (base >> 24) & 0xFF;
}

void gdt_install() {
    gdt_ptr gdtp = { .limit = (sizeof(gdt_entry) * 3) - 1, .base = reinterpret_cast<u32>(&gdt) };

    gdt_set_gate(0, 0, 0, 0, 0); // Null

    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel Code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Data

    load_gdt(reinterpret_cast<u32>(&gdtp));
}
