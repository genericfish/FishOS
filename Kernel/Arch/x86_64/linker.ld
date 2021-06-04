OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_bootstrap)

VIRTUAL_BASE_ADDR = 0xFFFFFFFF80000000 + 2M;

SECTIONS {
    . = VIRTUAL_BASE_ADDR;

    _kernel_start = .;

    .text ALIGN(4K) : AT (ADDR(.text) - VIRTUAL_BASE_ADDR) {
        _code = .;
        *(.multiboot)
        *(.text*)
    }

    .rodata ALIGN(4K) : AT (ADDR(.rodata) - VIRTUAL_BASE_ADDR) {
        _rodata = .;
        *(.rodata)

        . = ALIGN(8);
        __CTOR_LIST__ = .;
        *(.ctors)
        __CTOR_END__ = .;

        . = ALIGN(8);
        __DTOR_LIST__ = .;
        *(.dtors)
        __DTOR_END__ = .;

        . = ALIGN(8);
        __INIT_LIST__ = .;
        KEEP (*(.init_array))
        __INIT_END__ = .;

        . = ALIGN(8);
        __FINI_LIST__ = .;
        KEEP (*(.fini_array))
        __FINI_END__ = .;
    }

    .data ALIGN(4K) : AT (ADDR(.data) - VIRTUAL_BASE_ADDR) {
        _data = .;
        *(.data*)
    }

    .eh_frame ALIGN(4K) : AT (ADDR(.eh_frame) - VIRTUAL_BASE_ADDR) {
        _eh_frame = .;
        *(.eh_frame)
        *(.eh_frame_hdr)
    }

    _data_end = .;

    .bss ALIGN(4K) : AT (ADDR(.bss) - VIRTUAL_BASE_ADDR) {
        _bss = .;
        *(COMMON)
        *(.bss*)
    }

    _kernel_end = .;
}
