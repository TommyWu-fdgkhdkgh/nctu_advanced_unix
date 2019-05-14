;	comment!

        section .data
msg:    db      "hello, world!", 0x0a, 0

        section .text
        global _start
_start:

        mov     rax, 1
        mov     rdi, 1          ; stdout
        mov     rsi, msg        ; buffer
        mov     rdx, 14         ; length
        syscall                 ; syscall: write

        mov     rax, 60
        mov     rdi, 0          ; code
        syscall                 ; syscall: exit



