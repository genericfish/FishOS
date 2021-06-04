# Constants/Macros
.set KiB,               1024
.set VIRTUAL_BASE_ADDR, 0xFFFFFFFF80000000

# Multiboot consts
.set MULTIBOOT_ALIGN,       1<<0
.set MULTIBOOT_MEMINFO,     1<<1
.set MULTIBOOT_VIDEO_MODE,  1<<2
.set MULTIBOOT_FLAGS,       0x00010003 
.set MULTIBOOT_MAGIC,       0x1BADB002
.set MULTIBOOT_CHECKSUM,    -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

.code32

# Begin Multiboot
.section .multiboot
.align 4

multiboot_header:
# MULTIBOOT
.long MULTIBOOT_MAGIC
.long MULTIBOOT_FLAGS
.long MULTIBOOT_CHECKSUM

# MULTIBOOT_MEMINFO
.long (multiboot_header - VIRTUAL_BASE_ADDR)
.long (_kernel_start - VIRTUAL_BASE_ADDR)
.long (_data_end - VIRTUAL_BASE_ADDR)
.long (_kernel_end - VIRTUAL_BASE_ADDR)
.long (_bootstrap - VIRTUAL_BASE_ADDR)


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
    movl $(_bss - VIRTUAL_BASE_ADDR), %edi
    movl $(_kernel_end - VIRTUAL_BASE_ADDR), %ecx
    subl %edi, %ecx
    cld
    rep stosb

    popl %esi
    popl %edi

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

    # 64 bits!
    ljmp $0x08, $(_kernel - VIRTUAL_BASE_ADDR)

.data
.align 16
gdt:
    .quad 0
    .quad 0x20980000000000

gdt_end:
.align 16
gdt_ptr:
    .word gdt_end - gdt - 1
    .long gdt - VIRTUAL_BASE_ADDR

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

stack_bottom:
    .fill 4 * KiB, 1, 0
stack_top:

# Kernel entry
.code64
.global main
_kernel_high:
    movq $VIRTUAL_BASE_ADDR, %rax
    addq %rax, %rsp

    movq $0x0, init_pml4
    invlpg 0

    call main

_kernel_hang:
    hlt
    jmp _kernel_hang

_kernel:
    movabsq $_kernel_high, %rax
    jmp *%rax

.global load_idt
load_idt:
    cli
    cld
    lidt (%rdi)

    ret
