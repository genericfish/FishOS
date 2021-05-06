#pragma once

extern "C" {
struct regs {
    u32 gs, fs, es, ds;
    u32 edi, esi, eebp, esp, ebx, edx, ecx, eax;
    u32 intno, err;
    u32 eip, cs, eflags, useresp, ss;
};

void isr_install();
}
