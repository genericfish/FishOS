# Constants/Macros
.set KiB,               1024
.set VIRTUAL_BASE_ADDR, 0xC0000000

# Multiboot consts
.set MULTIBOOT_ALIGN,       1<<0
.set MULTIBOOT_MEMINFO,     1<<1
.set MULTIBOOT_VIDEO_MODE,  1<<2
.set MULTIBOOT_FLAGS,       MULTIBOOT_ALIGN | MULTIBOOT_MEMINFO
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

    # Enable PSE
    movl %cr4, %ecx
    orl $0x00000010, %ecx
    movl %ecx, %cr4

    # Enable paging and write-protect
    movl %cr0, %ecx
    orl $0x80010000, %ecx
    movl %ecx, %cr0

    # Jump to higher half
    lea 4f, %ecx
    jmp *%ecx

4:
    movl $0, boot_pgdir + 0    # Unmap identity mapping

    # Reload crc3 for TLB flush
    movl %cr3, %ecx
    movl %ecx, %cr3

    # Setup stack
    mov $stack_top, %esp

5:
    # Enter high-level kernel
    call main

    cli
1:  hlt
    jmp 1b

.set KERNEL_CODE, 0x08
.set KERNEL_DATA, 0x10

.global load_gdt
load_gdt:
    # Disable Interrupts
    cli
    cld

    # Load GDT Pointer
    movl 4(%esp), %eax
    lgdt (%eax)

    # Flush Registers
    call flush_gdt

    ret

flush_gdt:
    # Move GDT data segment into ax
    movw $KERNEL_DATA, %ax

    # Flush GDT registers
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    movw %ax, %fs
    movw %ax, %gs

    jmp $KERNEL_CODE, $flush_gdt_ret
flush_gdt_ret:
    ret

.global load_idt
load_idt:
    # Disable Interrupts
    cli
    cld

    # Load IDT Pointer
    movl 4(%esp), %eax
    lidt (%eax)

    ret

.global isr0
isr0:
//  Div. by 0
    push $0
    push $0

    jmp isr_common

.global isr1
isr1:
// Debug exception
    push $0
    push $1
    jmp isr_common

.global isr2
isr2:
// Non-maskable interrupt
    push $0
    push $2
    jmp isr_common

.global isr3
isr3:
// Breakpoint exception
    push $0
    push $3
    jmp isr_common

.global isr4
isr4:
// Detected overflow
    push $0
    push $4
    jmp isr_common

.global isr5
isr5:
// Out of bounds exception
    push $0
    push $5
    jmp isr_common

.global isr6
isr6:
// Invalid opcode
    push $0
    push $6
    jmp isr_common

.global isr7
isr7:
// No coprocessor exception
    push $0
    push $7
    jmp isr_common

.global isr8
isr8:
// Double fault
    push $8
    jmp isr_common

.global isr9
isr9:
// Coprocessor segment overrun exception
    push $0
    push $9
    jmp isr_common

.global isr10
isr10:
// Bad TSS exception
    push $10
    jmp isr_common

.global isr11
isr11:
// Segment not present exception
    push $11
    jmp isr_common

.global isr12
isr12:
// Stack fault exception
    push $12
    jmp isr_common

.global isr13
isr13:
// General protection fault exception
    push $13
    jmp isr_common

.global isr14
isr14:
// Page fault
    mov $(0xabadbabe), %ebx
    push $14
    jmp isr_common

.global isr15
isr15:
// Unknown interrupt exception
    push $0
    push $15
    jmp isr_common

.global isr16
isr16:
// Coprocessor fault exception
    push $0
    push $16
    jmp isr_common

.global isr17
isr17:
// Alignment check exception
    push $0
    push $17
    jmp isr_common

.global isr18
isr18:
// Machine check exception
    push $0
    push $18
    jmp isr_common
// Exceptions 19-31 are reserved

.global isr19
isr19:
    push $0
    push $19
    jmp isr_common

.global isr20
isr20:
    push $0
    push $20
    jmp isr_common

.global isr21
isr21:
    push $0
    push $21
    jmp isr_common

.global isr22
isr22:
    push $0
    push $22
    jmp isr_common

.global isr23
isr23:
    push $0
    push $23
    jmp isr_common

.global isr24
isr24:
    push $0
    push $24
    jmp isr_common

.global isr25
isr25:
    push $0
    push $25
    jmp isr_common

.global isr26
isr26:
    push $0
    push $26
    jmp isr_common

.global isr27
isr27:
    push $0
    push $27
    jmp isr_common

.global isr28
isr28:
    push $0
    push $28
    jmp isr_common

.global isr29
isr29:
    push $0
    push $29
    jmp isr_common

.global isr30
isr30:
    push $0
    push $30
    jmp isr_common

.global isr31
isr31:
    push $0
    push $31
    jmp isr_common

isr_common:
    pusha
    pushw %ds
    pushw %es
    pushw %fs
    pushw %gs

    movw $KERNEL_DATA, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    call fault_handler

    popw %gs
    popw %fs
    popw %es
    popw %ds
    popa

    addl $8, %esp
    iret

.global irq0
irq0:
    push $0
    push $32
    jmp irq_common

.global irq1
irq1:
    push $0
    push $33
    jmp irq_common

.global irq2
irq2:
    push $0
    push $34
    jmp irq_common

.global irq3
irq3:
    push $0
    push $35
    jmp irq_common

.global irq4
irq4:
    push $0
    push $36
    jmp irq_common

.global irq5
irq5:
    push $0
    push $37
    jmp irq_common

.global irq6
irq6:
    push $0
    push $38
    jmp irq_common

.global irq7
irq7:
    push $0
    push $39
    jmp irq_common

.global irq8
irq8:
    push $0
    push $40
    jmp irq_common

.global irq9
irq9:
    push $0
    push $41
    jmp irq_common

.global irq10
irq10:
    push $0
    push $42
    jmp irq_common

.global irq11
irq11:
    push $0
    push $43
    jmp irq_common

.global irq12
irq12:
    push $0
    push $44
    jmp irq_common

.global irq13
irq13:
    push $0
    push $45
    jmp irq_common

.global irq14
irq14:
    push $0
    push $46
    jmp irq_common

.global irq15
irq15:
    push $0
    push $47
    jmp irq_common

irq_common:
    pushal
    pushw %ds
    pushw %es
    pushw %fs
    pushw %gs

    movw $KERNEL_DATA, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    movl $0x09770748,0xB8000
    call irq_handler

    popw %gs
    popw %fs
    popw %es
    popw %ds
    popal

    addl $8, %esp
    iret
