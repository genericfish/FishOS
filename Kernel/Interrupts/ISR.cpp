#include <FSH/Types.h>

#include <Kernel/Interrupts/Interrupts.h>

#include <stdio.h>

extern "C" {
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

void isr_handler(reg_t r)
{
    printf("Exception %d\n", r.intn);

    asm volatile("cli");
    while (true)
        ;
}
}

void isr_install()
{
    idt_set_gate(0, reinterpret_cast<u64>(isr0), 0x8, 0x8E);
    idt_set_gate(1, reinterpret_cast<u64>(isr1), 0x8, 0x8E);
    idt_set_gate(2, reinterpret_cast<u64>(isr2), 0x8, 0x8E);
    idt_set_gate(3, reinterpret_cast<u64>(isr3), 0x8, 0x8E);
    idt_set_gate(4, reinterpret_cast<u64>(isr4), 0x8, 0x8E);
    idt_set_gate(5, reinterpret_cast<u64>(isr5), 0x8, 0x8E);
    idt_set_gate(6, reinterpret_cast<u64>(isr6), 0x8, 0x8E);
    idt_set_gate(7, reinterpret_cast<u64>(isr7), 0x8, 0x8E);
    idt_set_gate(8, reinterpret_cast<u64>(isr8), 0x8, 0x8E);
    idt_set_gate(9, reinterpret_cast<u64>(isr9), 0x8, 0x8E);
    idt_set_gate(10, reinterpret_cast<u64>(isr10), 0x8, 0x8E);
    idt_set_gate(11, reinterpret_cast<u64>(isr11), 0x8, 0x8E);
    idt_set_gate(12, reinterpret_cast<u64>(isr12), 0x8, 0x8E);
    idt_set_gate(13, reinterpret_cast<u64>(isr13), 0x8, 0x8E);
    idt_set_gate(14, reinterpret_cast<u64>(isr14), 0x8, 0x8E);
    idt_set_gate(15, reinterpret_cast<u64>(isr15), 0x8, 0x8E);
    idt_set_gate(16, reinterpret_cast<u64>(isr16), 0x8, 0x8E);
    idt_set_gate(17, reinterpret_cast<u64>(isr17), 0x8, 0x8E);
    idt_set_gate(18, reinterpret_cast<u64>(isr18), 0x8, 0x8E);
    idt_set_gate(19, reinterpret_cast<u64>(isr19), 0x8, 0x8E);
    idt_set_gate(20, reinterpret_cast<u64>(isr20), 0x8, 0x8E);
    idt_set_gate(21, reinterpret_cast<u64>(isr21), 0x8, 0x8E);
    idt_set_gate(22, reinterpret_cast<u64>(isr22), 0x8, 0x8E);
    idt_set_gate(23, reinterpret_cast<u64>(isr23), 0x8, 0x8E);
    idt_set_gate(24, reinterpret_cast<u64>(isr24), 0x8, 0x8E);
    idt_set_gate(25, reinterpret_cast<u64>(isr25), 0x8, 0x8E);
    idt_set_gate(26, reinterpret_cast<u64>(isr26), 0x8, 0x8E);
    idt_set_gate(27, reinterpret_cast<u64>(isr27), 0x8, 0x8E);
    idt_set_gate(28, reinterpret_cast<u64>(isr28), 0x8, 0x8E);
    idt_set_gate(29, reinterpret_cast<u64>(isr29), 0x8, 0x8E);
    idt_set_gate(30, reinterpret_cast<u64>(isr30), 0x8, 0x8E);
    idt_set_gate(31, reinterpret_cast<u64>(isr31), 0x8, 0x8E);
}
