#include <FSH/Types.h>

#include <Kernel/Interrupts/IDT.h>
#include <Kernel/Interrupts/IRQ.h>
#include <Kernel/Interrupts/ISR.h>
#include <Kernel/TTY/TTY.h>

#include <stdio.h>

extern "C" {
void kernel_main() {
    terminal_initialize();

    idt_install();
    isr_install();
    irq_install();

    asm ("sti");

    printf("      ===================          ====\n");
    printf("    ===                 ===       == ==\n");
    printf("  ===    ==               ====   ==  ==\n");
    printf("  =      ==                  =====   ==\n");
    printf("  ===                     ====  ==   ==\n");
    printf("    ===                 ===       == ==\n");
    printf("      ===================          ====\n");
    printf("            f i s h\n");
}
}
