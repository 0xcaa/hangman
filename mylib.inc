
section .text

read_string:
    mov     r8, rdx
    xor     rax, rax

    .l1:
        mov     rsi, rdi
        xor     rdi, rdi
        syscall
        add     rsi, rax
        mov     byte [rsi], 0
        sub     rsi, rax
        ;sub     rsi, 1

        mov     rax, rsi

        ret

        ;cmp     ecx, r8
        ;je      .end
        ;jmp     .l1


read_ch:
    push    0
    xor     rax, rax
    xor     rdi, rdi
    mov     rsi, rsp
    mov     rdx, 1
    syscall
    pop     rax
    ret

print_char:
    push    rdi ; push rdi since it's a caller-saved register
    mov     rdi, rsp ;  copy stackpointer to rdi, for function arg
    call    print_string ; print char
    pop     rdi ; pop rdi from stack
    ret
print_newline:
    mov     rdi, 10
    call    print_char
    ret


string_length:
    xor rax, rax ; zero-out rax
    .loop:
        cmp byte [rdi+rax], 0 ; look if NULL is att rax position
        je      .end ; if it is exit returning rax
        inc     rax ; inc rax to check every byte
        jmp .loop
    .end:
        ret
        
print_string:
    push    rdi ; push rdi since it's a caller-saved register
    call    string_length ; check to see how many bytes to print
    pop     rsi ; pop rdi into rsi since it is needed for syscall
    mov     rdx, rax ; move the strlen into rdx
    mov     rax, 1 ; write syscall code
    mov     rdi, 1 ; set FD
    syscall
    ret
        
        
string_equals:
    mov     al, byte[rdi] ; copy 1 byte of string to al
    cmp     al, byte[rsi] ; cmp with byte of other string
    jne     .no ; if not equal jmp to no
    inc     rsi ; inc to next byte
    inc     rdi
    test    al, al ; check if end of string
    jnz     string_equals ; restart loop if not end of string
    mov     rax, 1 ; return 1 if equal
    ret
    .no:
        xor     rax, rax ; return 0 if not equal
        ret

exit: 
    xor rax, rax ; zero-out rax
    mov rax, 60 ; exit syscall code
    syscall ; arg is exit code, rdi
    ret 


