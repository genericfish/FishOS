#include <FSH/Types.h>
#include <FSH/Utility.h>

#include <Kernel/Interrupts/Interrupts.h>

#include <stdio.h>
#include <sys/io.h>

namespace Interrupts {

void* irq_routines[16] = {};

extern "C" void irq_handler(Registers r)
{
    auto const index = r.intn - 32;

    if (index >= 16)
        return;

    auto handler = irq_routines[index];

    if (handler)
        reinterpret_cast<void (*)(Registers)>(handler)(r);

    if (r.intn >= 40)
        outb(0xA0, 0x20);

    outb(0x20, 0x20);
}

extern "C" void isr_handler(Registers r)
{
    printf("Exception 0x%x\n    CR2: 0x%llx\n    RIP: 0x%llx\n", r.intn, r.cr2, r.rip);

    asm volatile("cli");

    while (true)
        continue;
}

void irq_set(int irq, void (*handler)(Registers))
{
    if (irq < 0 || irq >= 16)
        return;

    irq_routines[irq] = reinterpret_cast<void*>(handler);
}

void irq_clear(int irq)
{
    if (irq < 0 || irq >= 16)
        return;

    irq_routines[irq] = 0;
}

void irq_remap()
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x00);
    outb(0xA1, 0x00);
}

extern "C" void idt_handlers();

void setup()
{
    IDT::setup();

    irq_remap();

    [&]<size_t... I>(index_sequence<I...>)
    {
        (IDT::idt_set_gate(I, reinterpret_cast<u64>(idt_handlers) + 0xA * I, 0x08, 0x8E), ...);
    }
    (make_index_sequence<48> {});
}

}
