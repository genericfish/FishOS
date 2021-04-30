// Constants/Macros
.set KiB,               1024
.set VIRTUAL_BASE_ADDR, 0xC0000000

// Multiboot consts
.set MULTIBOOT_ALIGN,       1<<0
.set MULTIBOOT_MEMINFO,     1<<1
.set MULTIBOOT_VIDEO_MODE,  1<<2
.set MULTIBOOT_FLAGS,       MULTIBOOT_ALIGN | MULTIBOOT_MEMINFO | MULTIBOOT_VIDEO_MODE
.set MULTIBOOT_MAGIC,       0x1BADB002
.set MULTIBOOT_CHECKSUM,    -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)


// Begin Multiboot
.section .multiboot.data
.align 4

// MULTIBOOT
.long MULTIBOOT_MAGIC
.long MULTIBOOT_FLAGS
.long MULTIBOOT_CHECKSUM

// MULTIBOOT_MEMINFO
.long 0
.long 0
.long 0
.long 0
.long 0

// MULTIBOOT_VIDEO_MODE
.long 0         // mode
.long 1280      // width
.long 720       // height
.long 32        // color depth

// Allocate stack
.section .stack, "aw", @nobits
stack_bottom:
.skip 16 * KiB
stack_top:

.section .bss, "aw", @nobits
    .align 4 * KiB
boot_pgdir:
    .skip 4 * KiB
boot_pgt1:
    .skip 4 * KiB

// o/ add more page tables here.

.section .text

// Kernel entry
.section .multiboot.text
.global _start
.type _start, @function
_start:
    cli
    cld

    movl $(boot_pgt1 - VIRTUAL_BASE_ADDR), %edi // Load physical address of boot_pgt1

    movl $0, %esi               // map address 0
    movl $1023, %ecx            // map first 1023 pages, 1024 is VGA text buffer

1:
    // Kernel mapping
    cmpl $_kernel_start, %esi
    jl 2f
    cmpl $(_kernel_end - VIRTUAL_BASE_ADDR), %esi
    jge 3f

    // Make physical address present and writable.
    // Side-effect .text and .rodata sections are writable

    movl %esi, %edx
    orl $0x003, %edx
    movl %edx, (%edi)

2:
    addl $(4 * KiB), %esi   // Size of page is 4 KiB
    addl $4, %edi           // Size of entry in boot_pgt1 is 4B
    loop 1b                 // Loop to next entry if not finished

3:
    // VGA video memory (present, writable)
    movl $(0xB8000 | 0x003), boot_pgt1 - VIRTUAL_BASE_ADDR + 1023 * 4

    // Map page table to virtual addresses 0x00000000 and VIRTUAL_BASE_ADDR
    movl $(boot_pgt1 - VIRTUAL_BASE_ADDR + 0x003), boot_pgdir - VIRTUAL_BASE_ADDR
    movl $(boot_pgt1 - VIRTUAL_BASE_ADDR + 0x003), boot_pgdir - VIRTUAL_BASE_ADDR + 768 * 4

    // Set cr3 to address of boot_pgdir
    movl $(boot_pgdir - VIRTUAL_BASE_ADDR), %ecx
    movl %ecx, %cr3

    // Enable paging and write-protect
    movl %cr0, %ecx
    orl $0x80010000, %ecx
    movl %ecx, %cr0

    // Jump to higher half
    lea 4f, %ecx
    jmp *%ecx

4:
    movl $0, boot_pgdir    // Unmap identity mapping

    // Reload crc3 for TLB flush
    movl %cr3, %ecx
    movl %ecx, %cr3

    // Setup stack
    mov $stack_top, %esp

    // Enter high-level kernel
    call kernel_main

    // Infinite loop
    cli
1:  hlt
    jmp 1b
