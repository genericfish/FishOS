#include <FSH/Types.h>

#include <Kernel/Interrupts/IDT.h>
#include <Kernel/Interrupts/IRQ.h>
#include <Kernel/Interrupts/ISR.h>

#include <sys/io.h>

extern "C" {
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void* irq_routines[16] = {};

void irq_set(int irq, void* handler) {
    if (irq < 0 || irq >= 16)
        return;

    irq_routines[irq] = handler;
}

void irq_clear(int irq) {
    if (irq < 0 || irq >= 16)
        return;

    irq_routines[irq] = 0;
}

void irq_remap() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void irq_install() {
    irq_remap();

    idt_set_gate(32, reinterpret_cast<u32>(irq0), 0x08, 0x8E);
    idt_set_gate(33, reinterpret_cast<u32>(irq1), 0x08, 0x8E);
    idt_set_gate(34, reinterpret_cast<u32>(irq2), 0x08, 0x8E);
    idt_set_gate(35, reinterpret_cast<u32>(irq3), 0x08, 0x8E);
    idt_set_gate(36, reinterpret_cast<u32>(irq4), 0x08, 0x8E);
    idt_set_gate(37, reinterpret_cast<u32>(irq5), 0x08, 0x8E);
    idt_set_gate(38, reinterpret_cast<u32>(irq6), 0x08, 0x8E);
    idt_set_gate(39, reinterpret_cast<u32>(irq7), 0x08, 0x8E);
    idt_set_gate(40, reinterpret_cast<u32>(irq8), 0x08, 0x8E);
    idt_set_gate(41, reinterpret_cast<u32>(irq9), 0x08, 0x8E);
    idt_set_gate(42, reinterpret_cast<u32>(irq10), 0x08, 0x8E);
    idt_set_gate(43, reinterpret_cast<u32>(irq11), 0x08, 0x8E);
    idt_set_gate(44, reinterpret_cast<u32>(irq12), 0x08, 0x8E);
    idt_set_gate(45, reinterpret_cast<u32>(irq13), 0x08, 0x8E);
    idt_set_gate(46, reinterpret_cast<u32>(irq14), 0x08, 0x8E);
    idt_set_gate(47, reinterpret_cast<u32>(irq15), 0x08, 0x8E);
}

void irq_handler(regs* r) {
    void* handler = irq_routines[r->intno - 32];

    if (handler)
        reinterpret_cast<void(*)(void*)>(handler)(r);

    if (r->intno >= 40)
        outb(0xA0, 0x20);

    outb(0x20, 0x20);
}
}
