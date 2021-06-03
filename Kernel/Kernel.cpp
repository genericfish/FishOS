#include <FSH/Platform.h>
#include <FSH/Types.h>

#include <Kernel/Interrupts/Interrupts.h>
#include <Kernel/TTY/TTY.h>

#include <stdio.h>
#include <sys/io.h>

extern "C" {

void main()
{
    terminal_initialize();

    gdt_install();
    idt_install();
    isr_install();
    irq_install();

    asm("sti");

    printf("      ===================          ====\n");
    printf("    ===                 ===       == ==\n");
    printf("  ===    ==               ====   ==  ==\n");
    printf("  =      ==                  =====   ==\n");
    printf("  ===                     ====  ==   ==\n");
    printf("    ===                 ===       == ==\n");
    printf("      ===================          ====\n");
    printf("            f i s h\n");

    while (true)
        ;
}
}
