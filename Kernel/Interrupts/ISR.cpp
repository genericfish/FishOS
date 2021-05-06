#include <FSH/Types.h>

#include <Kernel/Interrupts/IDT.h>
#include <Kernel/Interrupts/ISR.h>

#include <stdio.h>

struct regs {
    u32 gs, fs, es, ds;
    u32 edi, esi, eebp, esp, ebx, edx, ecx, eax;
    u32 intno, err;
    u32 eip, cs, eflags, useresp, ss;
};

extern "C" {

extern u32 CODE_SEG;
extern u32 DATA_SEG;

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void isr_install() {
    idt_set_gate(0, (unsigned)isr0, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr1, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr2, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr3, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr4, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr5, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr6, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr7, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr8, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr9, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr10, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr11, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr12, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr13, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr14, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr15, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr16, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr17, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr18, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr19, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr20, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr21, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr22, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr23, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr24, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr25, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr26, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr27, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr28, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr29, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr30, CODE_SEG, 0x8E);
    idt_set_gate(0, (unsigned)isr31, CODE_SEG, 0x8E);
}

void fault_handler(regs* r) {
    if (r->intno < 32) {
        printf("Exception %d\n", r->intno);
        while (true);
    }
}

}