# Constants/Macros
.set KiB,               1024
.set VIRTUAL_BASE_ADDR, 0xC0000000

# Multiboot consts
.set MULTIBOOT_ALIGN,       1<<0
.set MULTIBOOT_MEMINFO,     1<<1
.set MULTIBOOT_VIDEO_MODE,  1<<2
.set MULTIBOOT_FLAGS,       MULTIBOOT_ALIGN | MULTIBOOT_MEMINFO | MULTIBOOT_VIDEO_MODE
.set MULTIBOOT_MAGIC,       0x1BADB002
.set MULTIBOOT_CHECKSUM,    -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)


# Begin Multiboot
.section .multiboot
.align 4

# MULTIBOOT
.long MULTIBOOT_MAGIC
.long MULTIBOOT_FLAGS
.long MULTIBOOT_CHECKSUM

# MULTIBOOT_MEMINFO
.long 0
.long 0
.long 0
.long 0
.long 0

# MULTIBOOT_VIDEO_MODE
.long 0         # mode
.long 1280      # width
.long 720       # height
.long 32        # color depth

# Allocate stack
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

# o/ add more page tables here.

.section .text

# Kernel entry
.global _start
.type _start, @function
_start:
    cli
    cld

    movl $(boot_pgt1 - VIRTUAL_BASE_ADDR), %edi # Load physical address of boot_pgt1

    movl $0, %esi               # map address 0
    movl $1023, %ecx            # map first 1023 pages, 1024 is VGA text buffer

1:
    # Kernel mapping
    cmpl $_kernel_start, %esi
    jl 2f
    cmpl $(_kernel_end - VIRTUAL_BASE_ADDR), %esi
    jge 3f

    # Make physical address present and writable.
    # Side-effect .text and .rodata sections are writable

    movl %esi, %edx
    orl $0x003, %edx
    movl %edx, (%edi)

2:
    addl $(4 * KiB), %esi   # Size of page is 4 KiB
    addl $4, %edi           # Size of entry in boot_pgt1 is 4B
    loop 1b                 # Loop to next entry if not finished

3:
    # VGA video memory (present, writable)
    movl $(0xB8000 | 0x003), boot_pgt1 - VIRTUAL_BASE_ADDR + 1023 * 4

    # Map page table to virtual addresses 0x00000000 and VIRTUAL_BASE_ADDR
    movl $(boot_pgt1 - VIRTUAL_BASE_ADDR + 0x003), boot_pgdir - VIRTUAL_BASE_ADDR
    movl $(boot_pgt1 - VIRTUAL_BASE_ADDR + 0x003), boot_pgdir - VIRTUAL_BASE_ADDR + 768 * 4

    # Set cr3 to address of boot_pgdir
    movl $(boot_pgdir - VIRTUAL_BASE_ADDR), %ecx
    movl %ecx, %cr3

    # Enable paging and write-protect
    movl %cr0, %ecx
    orl $0x80010000, %ecx
    movl %ecx, %cr0

    # Jump to higher half
    lea 4f, %ecx
    jmp *%ecx

4:
    movl $0, boot_pgdir    # Unmap identity mapping

    # Reload crc3 for TLB flush
    movl %cr3, %ecx
    movl %ecx, %cr3

    # Setup stack
    mov $stack_top, %esp

    # Load GDT
    call load_gdt

5:
    # Enter high-level kernel
    call kernel_main

    # Infinite loop
    cli
1:  hlt
    jmp 1b

gdt_start:
gdt_null:
    .quad 0

gdt_code:
    .word 0xFFFF        # limit [0, 15]
    .word 0             # base [0, 15]
    .byte 0             # base [16, 23]
    .byte 0b10011010    # flags
    .byte 0b11001111    # flags, limit [16, 19]
    .byte 0             # base [24, 31]

gdt_data:
    .word 0xFFFF        # limit
    .word 0             # base
    .byte 0             # base
    .byte 0b10010010    # flags
    .byte 0b11001111    # flags, limit
    .byte 0             # base

gdt_end:
gdt_descriptor:
    .word gdt_end - gdt_start - 1
    .long gdt_start

.set CODE_SEG, gdt_code - gdt_start
.set DATA_SEG, gdt_data - gdt_start

load_gdt:
    # Load GDT
    lgdt (gdt_descriptor)

    # Clear EAX register
    xor %eax, %eax

    # Move GDT data segment into ax
    mov $(DATA_SEG), %ax

    # Flush GDT registers
    mov %eax, %ds
    mov %eax, %es
    mov %eax, %ss
    mov %eax, %fs
    mov %eax, %gs

    ret

.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31

isr0:
//  Div. by 0
    cli
    push 0
    push 0

    jmp isr_common

isr1:
// Debug exception
    cli
    push 0
    push 1
    jmp isr_common

isr2:
// Non-maskable interrupt
    cli
    push 0
    push 2
    jmp isr_common

isr3:
// Breakpoint exception
    cli
    push 0
    push 3
    jmp isr_common

isr4:
// Detected overflow
    cli
    push 0
    push 4
    jmp isr_common

isr5:
// Out of bounds exception
    cli
    push 0
    push 5
    jmp isr_common

isr6:
// Invalid opcode
    cli
    push 0
    push 6
    jmp isr_common

isr7:
// No coprocessor exception
    cli
    push 0
    push 7
    jmp isr_common

isr8:
// Double fault
    cli
    push 8
    jmp isr_common

isr9:
// Coprocessor segment overrun exception
    cli
    push 0
    push 9
    jmp isr_common

isr10:
// Bad TSS exception
    cli
    push 10
    jmp isr_common

isr11:
// Segment not present exception
    cli
    push 11
    jmp isr_common

isr12:
// Stack fault exception
    cli
    push 12
    jmp isr_common

isr13:
// General protection fault exception
    cli
    push 13
    jmp isr_common

isr14:
// Page fault
    cli
    push 14
    jmp isr_common

isr15:
// Unknown interrupt exception
    cli
    push 0
    push 15
    jmp isr_common

isr16:
// Coprocessor fault exception
    cli
    push 0
    push 16
    jmp isr_common

isr17:
// Alignment check exception
    cli
    push 0
    push 17
    jmp isr_common

isr18:
// Machine check exception
    cli
    push 0
    push 18
    jmp isr_common

// Exceptions 19-31 are reserved
isr19:
    cli
    push 0
    push 19
    jmp isr_common

isr20:
    cli
    push 0
    push 20
    jmp isr_common

isr21:
    cli
    push 0
    push 21
    jmp isr_common

isr22:
    cli
    push 0
    push 22
    jmp isr_common

isr23:
    cli
    push 0
    push 23
    jmp isr_common

isr24:
    cli
    push 0
    push 24
    jmp isr_common

isr25:
    cli
    push 0
    push 25
    jmp isr_common

isr26:
    cli
    push 0
    push 26
    jmp isr_common

isr27:
    cli
    push 0
    push 27
    jmp isr_common

isr28:
    cli
    push 0
    push 28
    jmp isr_common

isr29:
    cli
    push 0
    push 29
    jmp isr_common

isr30:
    cli
    push 0
    push 30
    jmp isr_common

isr31:
    cli
    push 0
    push 31
    jmp isr_common

isr_common:
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov (DATA_SEG), %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax
    push %eax

    call fault_handler

    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

    add $(8), %esp
    sti
    iret
