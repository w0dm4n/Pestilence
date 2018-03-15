section .text
    global ft_hello
ft_hello:
    mov     rax, 1
    mov     rsi, rdi
    mov     rdi, 1
    mov     rdx, 13
    syscall
    ret

