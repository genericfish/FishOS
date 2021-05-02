.global _init
.section .init
.type _init, @function
_init:
    push %ebp
    movl %esp, %ebp

.global _fini
.section .fini
.type _fini, @function
_fini:
    push %ebp
    movl %esp, %ebp
