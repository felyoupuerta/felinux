[bits 32]
[extern kernel_main]
[global _start]

section .multiboot
    align 4
    dd 0x1BADB002              ; magic
    dd 0x00000003              ; flags
    dd -(0x1BADB002 + 0x00000003) ; checksum

section .text
_start:
    mov esp, stack_top
    
    push 0
    popfd
    
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:
