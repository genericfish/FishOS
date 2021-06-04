#pragma once

#include <FSH/Platform.h>
#include <FSH/Types.h>

struct regs {
    u16 gs;
    u16 fs;
    u16 es;
    u16 ds;
    u32 edi;
    u32 esi;
    u32 ebp;
    u32 esp;
    u32 ebx;
    u32 edx;
    u32 ecx;
    u32 eax;
    u8 intno;
    u8 err;
};

struct PACKED IDT_Entry {
    u16 base_lo;
    u16 sel;
    u8 ist;
    u8 flags;
    u16 base_med;
    u32 base_hi;
    u32 zero;
};

struct PACKED IDT_ptr {
    u16 limit;
    u64 base;
};

extern "C" {
void irq_install();
void irq_set(int, void (*)(regs));
void isr_install();
}

IDT_Entry const& idt_get_gate(size_t);
void idt_set_gate(u8, u64, u16, u8);
void idt_install();
void gdt_install();
