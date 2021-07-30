#include <FSH/Platform.h>
#include <FSH/Types.h>
#include <FSH/Utility.h>

#include <Kernel/Interrupts/Interrupts.h>
#include <Kernel/TTY/TTY.h>

#include <Kernel/Memory/Paging.h>
#include <Kernel/Memory/Address.h>

#include <stdio.h>
#include <sys/io.h>

using namespace Address;
using namespace Memory;

u64 ticks = 0;

void timer(Interrupts::Registers)
{
    if (ticks++ % 18 == 0)
        printf("%lld seconds.\n", ticks / 18);
}

extern "C" int main()
{
    TTY::clear(VGA::vga_entry_color(VGA::WHITE, VGA::BLUE));

    Interrupts::setup();

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

    while (true)
        continue;

    return 0;
}
