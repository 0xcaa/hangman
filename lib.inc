;DEFAULT REL

section .bss
termios:    resb 36
stdin_fd:   equ 0
ICANON:     equ 1<<1

ECHO:       equ 1<<3

section .text
 
exit: 
    xor rax, rax ; zero-out rax
    mov rax, 60 ; exit syscall code
    syscall ; arg is exit code, rdi
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

print_char:
    push    rdi ; push rdi since it's a caller-saved register
    mov     rdi, rsp ;  copy stackpointer to rdi, for function arg
    call    print_string ; print char
    pop     rdi ; pop rdi from stack
    ret

print_newline:
    mov     rdi, 10 ; move newline code into rdi
    jmp     print_char ; jmp insted of call since it os faster. it does not return to 
                       ; caller after its done since jmp does not push rip

print_uint:
    mov     rax, rdi ; copy the number to be printed into rax
    mov     rdi, rsp ; copy SP into rdi
    push    0 ;  zero out 8 bytes on stack
    sub     rsp, 16 ; allocate 16 bytes on stack,

    dec     rdi; dec rdi which points twords first zeroed out byte
    mov     r8, 10 ; move 10 into r8

    .loop:
        xor     rdx, rdx ; zero-out rdx
        div     r8 ; rax/10, al = qoutient, ah, remainder
        or      dl, 0x30 ; logical or with 48, stores 0x30 in dl
        dec     rdi ; dec rdi, points 1 byte higher each loop
        mov     [rdi], dl ; copys dl byte into value of rdi
        test    rax, rax ; check if rax is zero
        jnz     .loop ; if not restart loop

        call    print_string  ; print int

        add     rsp, 24 ; restor stack 
    ret

print_int:
    test    rdi, rdi ; check if SF is set. if so rdi is negativ
    jns     print_uint ; if not signed jmp right function
    push    rdi ; push rdi so it can print minus sign
    mov     rdi, '-' ; argument for print_char
    call    print_char ; use call since we need to get back here
    pop     rdi ; get the original value back
    neg     rdi ; reverse the sign-bits
    jmp     print_uint ; print it like unsigned int
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

read_char:
    push    0 ; allocate and zero-out 1 byte
    xor     rax, rax ; syscall code
    xor     rdi, rdi ; FD
    mov     rsi, rsp ; rsi points to the 0 in stack. buffer
    mov     rdx, 1 ; size to read
    syscall
    pop     rax ; pop first value that was read to rax as return value
    ret 

read_word:
    push    r14 ; callee-saved register
    push    r15 ; callee-saved register
    xor     r14, r14 ; zero-out  r14
    mov     r15, rsi ; rsi is length of word to read. rdi is buffer to store word at
    dec     r15 ; allocate 1 byte

    .A: ; this is to remove all whitespaces befor it get to a char
        push    rdi ; caller-saved register
        call    read_char ; get a char in rax
        pop     rdi
        cmp     al, ' ' ; check if char in white space
        je      .A ; if it is a white space get another char. restart loop
        cmp     al, 10 ; check if it's a newline
        je      .A 
        cmp     al, 13 ; check if it's a CR
        je      .A
        cmp     al, 9 ; check if it's a tab
        je      .A
        test    al, al ; check if it's a NULL
        jz      .C ; jmp to c if so
    .B: ; writes char to buffer
        mov     byte [rdi + r14], al ; put char in r14 position
        inc     r14 ; inc r14 1 byte to fit next char

        push    rdi ; caller-saved reg
        call    read_char ; get another char
        pop     rdi 
        cmp     al, ' ' ; check if char is whitespace
        je      .C ; then jmp to c
        cmp     al, 10 ; check newline
        je      .C
        cmp     al, 13 ; check if cr
        je      .C
        cmp     al, 9 ; check if tab
        je      .C
        test    al, al ; check if al is NULL
        jz      .C ; jmp c if sp
        cmp     r14, r15 ; prevent buffer overflow
        je      .D ; if it's reading more then allowed exit

        jmp     .B ; restart loop

    .C: ; exit successful
        mov     byte[rdi+r14], 0 ; write null to end of word
        mov     rax, rdi ; return buffer in rax

        mov     rdx, r14 ; return amount read
        pop     r15
        pop     r14
        ret

    .D:
        xor     rax, rax ; return 0
        pop     r15
        pop     r14
        ret

    ret
parse_uint:
    mov     r8, 10
    xor     rax, rax
    xor     rcx, rcx
    .loop: ; get 1 number at a time from a string
        movzx   r9, byte[rdi + rcx] ; copy zero ext byte into r9, indexed by rcx
        cmp     r9b, '0' ; check if the byte is between 0 and 9
        jb      .end ; if not go to end
        cmp     r9b, '9' 
        ja      .end
        xor     rdx, rdx ; zero-out rdx
        mul     r8 ; every time it gets new num mul the rax with 10
        and     r9b, 0x0f ; clear upper 4 bits of the byte
        add     rax, r9 ; add number to rax, since it only gets 1 number at a time
        inc     rcx ; rcx in counter on numbers parsed and used to index string
        jmp     .loop ; restart loop
    .end:
        mov     rdx, rcx ; the number is returned in rax, char count in rdx
    ret


parse_int:
    mov     al, byte[rdi] ; get first byte from arg
    cmp     al, '-' ; check if byte is signed
    je      .signed
    jmp     parse_uint ; if not signed jmp to unsigned int parser
    .signed:
        inc     rdi ; move index 1
        call    parse_uint ; parse the number
        neg     rax ; reverse the sign
        test    rdx, rdx ; check if parse_uint returned 0 in char count or not
        jz      .error ; if so go to error. happens when there is no num at index

        inc     rdx ; inc 1 for sign
        ret

    .error:
        xor     rax, rax ; return 0
        ret

string_copy:
    push    rdi ; source strning
    push    rsi ; dest
    push    rdx ; dest length
    call    string_length ; check length of source
    pop     rdx ; caller-saved registers
    pop     rsi
    pop     rdi

    cmp     rax, rdx ; check if source is bigger then buffer
    jae     .toolong ; if so, end

    push    rsi
    .loop:
        mov     dl, byte[rdi] ; copy 1 byte at a time
        mov     byte[rsi], dl
        inc     rdi ; get next char
        inc     rsi ; go to next index in buffer
        test    dl, dl ; chek if we got a NULL byte
        jnz     .loop ; if so copy is done

    pop     rax ; return address of first byte of string
    ret
    .toolong:
    xor     rax, rax ; return 0
    ret


write_stdin_termios:
    push    rbx

    mov     eax, 36h
    mov     ebx, stdin_fd
    mov     ecx, 5402h
    mov     edx, termios
    int     80h

    pop     rbx
    ret



read_stdin_termios:
        push rbx

        mov eax, 36h
        mov ebx, stdin_fd
        mov ecx, 5401h
        mov edx, termios
        int 80h            ; ioctl(0, 0x5401, termios)

        pop rbx
        ret
        
canonical_on:
        call read_stdin_termios

        ; set canonical bit in local mode flags
        or dword [termios+12], ICANON

        call write_stdin_termios
        ret
        
canonical_off:
    call    read_stdin_termios

    and     dword [termios+12], ~ICANON

    call    write_stdin_termios
    ret

read_char_noe:
    call    canonical_off
    call    read_char
    push    rax
    call    canonical_on
    pop     rax
    ret


