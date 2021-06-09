#include <FSH/Platform.h>
#include <FSH/Types.h>
#include <FSH/Utility.h>

#include <Kernel/Interrupts/Interrupts.h>
#include <Kernel/TTY/TTY.h>

#include <stdio.h>
#include <sys/io.h>

static u64 ticks = 0;

void timer(reg_t)
{
    if (ticks++ % 18 == 0)
        printf("%lld seconds.\n", ticks / 18);
}

extern "C" int main()
{
    TTY::clear(VGA::vga_entry_color(VGA::WHITE, VGA::BLUE));

    idt_install();
    isr_install();
    irq_install();

    irq_set(0, timer);

    asm("sti");

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
    [&]<size_t... I>(index_sequence<I...>)
    {
        (printf("%d\n", I), ...);
    }
    (make_index_sequence<5> {});

    // TEST: Div by 0
    // for (auto i = 0; i < 5; i++)
    //     printf("%d\n", 120 / i);

    // TEST: Pagefaults
    // *reinterpret_cast<u32*>(0x1BADBABE) = 0xDEADBEEF;

    while (true)
        continue;

    return 0;
}
