.global _start
.section .text

_start:

    mov x8, #64
    mov x0, #1
    ldr x1, =message
    mov x2, #14
    svc 0

    mov x8, #0x5d
    mov x0, #0
    svc 0

.section .data
    message:
        .ascii "hello, bitches\n" 
