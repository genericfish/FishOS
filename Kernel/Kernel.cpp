#include <FSH/Types.h>
#include <Kernel/TTY/TTY.h>

#include <stdio.h>

extern "C" {
void kernel_main() {
    terminal_initialize();

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
