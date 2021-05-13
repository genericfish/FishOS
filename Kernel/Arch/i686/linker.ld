ENTRY(_start)

VIRTUAL_BASE_ADDR = 3072M;

SECTIONS {
    . = VIRTUAL_BASE_ADDR + 1M;

    _kernel_start = .;

    .text ALIGN(4K) : AT (ADDR(.text) - VIRTUAL_BASE_ADDR) {
        *(.multiboot)
        *(.text)
    }

    .rodata ALIGN(4K) : AT (ADDR(.rodata) - VIRTUAL_BASE_ADDR) {
        *(.rodata)
    }

    .data ALIGN(4K) : AT (ADDR(.data) - VIRTUAL_BASE_ADDR) {
        *(.data)
    }

    .bss ALIGN(4K) : AT (ADDR(.bss) - VIRTUAL_BASE_ADDR) {
        *(COMMON)
        *(.bss)
        *(.stack)
    }

    _kernel_end = .;
}
