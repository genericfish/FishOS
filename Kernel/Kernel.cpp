#include <FSH/Platform.h>
#include <FSH/Types.h>
#include <FSH/Utility.h>

#include <Kernel/Interrupts/Interrupts.h>
#include <Kernel/TTY/TTY.h>

#include <stdio.h>
#include <sys/io.h>

extern "C" int main()
{
    TTY::clear(VGA::vga_entry_color(VGA::WHITE, VGA::BLUE));

    // TODO/FIXME: Implement interrupts!
    idt_install();
    // isr_install();
    // irq_install();

    // irq_set(0, timer);

    // asm("sti");

    printf("      ===================          ====\n");
    printf("    ===                 ===       == ==\n");
    printf("  ===    ==               ====   ==  ==\n");
    printf("  =      ==                  =====   ==\n");
    printf("  ===                     ====  ==   ==\n");
    printf("    ===                 ===       == ==\n");
    printf("      ===================          ====\n");
    printf("            f i s h\n");

    // TEST: printf formatting
    printf("Expect -16: %d\nExpect 29: %d\n", -16, 29);
    printf("Expect DEADBEEF: %x\n", 0xdeadbeef);
    printf("Expect 777: %o\n", 0777);
    printf("Expect 11: %b\n", 3);
    printf("Address of idt_install(): %llx\n", reinterpret_cast<u64>(&idt_install));

    // TEST: Compile time loops using integer packs (remove this later).
    [&]<size_t... I>(index_sequence<I...>) {
        (printf("%d\n", I), ...);
    }(make_index_sequence<5>{});

    while (true)
        ;

    return 0;
}
