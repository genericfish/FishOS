#pragma once

#include <FSH/Platform.h>
#include <FSH/Types.h>

struct PACKED reg_t {
    u64 cr0, cr2, cr3, cr4;

    u64 r11, r10, r9, r8;
    u64 rbp, rsi, rdi;
    u64 rdx, rcx, rbx, rax;

    u64 intn, err;

    u64 rip, cs, flags, rsp, ss;
};

struct PACKED IDT_Entry {
    u16 base_lo;
    u16 sel;
    u8 ist;
    u8 flags;
    u16 base_mid;
    u32 base_hi;
    u32 zero;
};

struct PACKED IDT_Descriptor {
    u16 limit;
    IDT_Entry* base;
};

void idt_install();
void isr_install();
void irq_install();

void irq_remap();
void irq_clear(int);
void irq_set(int, void (*)(reg_t));

IDT_Entry const& idt_get_gate(size_t);
void idt_set_gate(u8, u64, u16, u8);
