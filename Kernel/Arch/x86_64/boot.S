# Constants/Macros
.set KiB,               1024
.set VIRTUAL_BASE_ADDR, 0xFFFFFFFF80000000

.macro pushall
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx

    pushq %rdi
    pushq %rsi
    pushq %rbp

    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
.endm

.macro pushcr
    mov %cr4, %rax
    pushq %rax

    mov %cr3, %rax
    pushq %rax

    mov %cr2, %rax
    pushq %rax

    mov %cr0, %rax
    pushq %rax
.endm

.macro popall
    popq %r11
    popq %r10
    popq %r9
    popq %r8

    popq %rbp
    popq %rsi
    popq %rdi

    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
.endm

.macro popcr
    popq %rax
    popq %rax
    popq %rax
    popq %rax
.endm

.macro isr n
isr\n\():
    cli
    pushq $0
    pushq $(\n)

    jmp isr_common
.endm

.macro isrc n
isr\n\():
    cli
    pushq $(\n)
    nop
    nop

    jmp isr_common
.endm

.macro irq m, n
.global irq\m
irq\m\():
    cli
    pushq $0
    pushq $(\n)

    jmp irq_common
.endm

.macro idt_common name
\name\()_common:
    pushall
    pushcr

    call \name\()_handler

    popcr
    popall

    addq $16, %rsp
    sti
    iretq
.endm

# Multiboot consts
.set MULTIBOOT_PAGE_ALIGN,  1 << 0
.set MULTIBOOT_MEMORY_INFO, 1 << 1
.set MULTIBOOT_VIDEO_MODE,  1 << 2
.set MULTIBOOT_TEXT_MODE,   1 << 16
.set MULTIBOOT_FLAGS,       MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_TEXT_MODE
.set MULTIBOOT_MAGIC,       0x1BADB002
.set MULTIBOOT_CHECKSUM,    -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

.code32

# Begin Multiboot
.section .multiboot
.align 4

multiboot_header:
.long MULTIBOOT_MAGIC
.long MULTIBOOT_FLAGS
.long MULTIBOOT_CHECKSUM

multiboot_memory:
.long (multiboot_header - VIRTUAL_BASE_ADDR)
.long (_kernel_start - VIRTUAL_BASE_ADDR)
.long (_data_end - VIRTUAL_BASE_ADDR)
.long (_kernel_end - VIRTUAL_BASE_ADDR)
.long (_bootstrap - VIRTUAL_BASE_ADDR)

multiboot_video:
.long 0x00000000 # mode
.long       1280 # width
.long       1024 # height
.long         32 # depth

.section .text
.global _bootstrap, init_pml4, init_pdp, init_pd
_bootstrap:
    cli

    lgdt (gdt_ptr - VIRTUAL_BASE_ADDR)

    movl $(stack_top - VIRTUAL_BASE_ADDR), %esp

    pushl $0
    popf

    pushl %eax
    pushl %ebx

    xorl %eax, %eax
    movl $(_bss - VIRTUAL_BASE_ADDR), %ebp
    movl $(_kernel_end - VIRTUAL_BASE_ADDR), %ecx
    subl %ebp, %ecx
    cld
    rep stosb

    # Disable Paging
    movl %cr0, %eax
    andl $0x7FFFFFFF, %eax
    movl %eax, %cr0

    # Enable PAE
    movl %cr4, %eax
    orl $0x00000020, %eax
    movl %eax, %cr4

    # Load L4 PD
    movl $(init_pml4 - VIRTUAL_BASE_ADDR), %eax
    movl %eax, %cr3

    # Set LME in IA32_EFER
    movl $0xC0000080, %ecx
    rdmsr

    # Set PAE and SYSENTER
    orl $0x00000101, %eax
    wrmsr

    # Enable Paging
    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0

    popl %esi
    popl %edi

    # 64 bits!
    ljmp $kcode, $(_kernel - VIRTUAL_BASE_ADDR)

# Kernel entry
.code64
.global main
_kernel_high:
    movq $VIRTUAL_BASE_ADDR, %rax
    addq %rax, %rsp

    movq $0x0, init_pml4
    invlpg 0

    lgdt (gdt_ptr64)

    mov $kdata, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    call main

_kernel_hang:
    hlt
    jmp _kernel_hang

_kernel:
    movabsq $_kernel_high, %rax
    jmp *%rax

idt_common isr
idt_common irq

.global idt_handlers
idt_handlers:
isr 0
isr 1
isr 2
isr 3
isr 4
isr 5
isr 6
isr 7
isrc 8
isr 9
isrc 10
isrc 11
isrc 12
isrc 13
isrc 14
isr 15
isr 16
isr 17
isr 18
isr 19
isr 20
isr 21
isr 22
isr 23
isr 24
isr 25
isr 26
isr 27
isr 28
isr 29
isr 30
isr 31
irq 0, 32
irq 1, 33
irq 2, 34
irq 3, 35
irq 4, 36
irq 5, 37
irq 6, 38
irq 7, 39
irq 8, 40
irq 9, 41
irq 10, 42
irq 11, 43
irq 12, 44
irq 13, 45
irq 14, 46
irq 15, 47

.data
.align 16
gdt:
    .quad 0
gdt_kcode:
    .quad 0x00209A0000000000
gdt_kdata:
    .quad 0x0000920000000000

gdt_end:

.align 16
gdt_ptr:
    .word gdt_end - gdt - 1
    .long gdt - VIRTUAL_BASE_ADDR

gdt_ptr64:
    .word gdt_end - gdt - 1
    .quad gdt

.set kcode, gdt_kcode - gdt
.set kdata, gdt_kdata - gdt

.align 4 * KiB
init_pml4:
    .quad init_pdp - VIRTUAL_BASE_ADDR + 3
    .fill 510, 8, 0
    .quad init_pdp - VIRTUAL_BASE_ADDR + 3

init_pdp:
    .quad init_pd - VIRTUAL_BASE_ADDR + 3
    .fill 509, 8, 0
    .quad init_pd - VIRTUAL_BASE_ADDR + 3
    .fill 1, 8, 0

init_pd:
    .quad 0x83
    .quad 0x200083
    .fill 510, 8, 0

.section .stack, "aw", @nobits
.align 16
stack_bottom:
    .skip 32 * KiB
stack_top:
