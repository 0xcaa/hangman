%include "mylib.inc"
%include "hangmanart.inc"

%define word_len 30

global _start

section .data
;ground:      db        "----------------------------------------------------------------------------------",10,0
start_msg:   db        "Hangman! type a word to guess: ",10,0
guess_msg:   db        "Guess a letter or whole word: ",0
win_msg:     db        "You win!!",10,0
lose_msg:    db        "You lose :(",10,0

password:       times word_len db 0
guess:          times word_len db '_'

section .text

_start:
    ;mov     rdi, ground
    ;call    print_string
    mov     rdi, start_msg
    call    print_string

    ; get password
    xor     rsi, rsi
    mov     rdi, password
    mov     rdx, word_len
    call    read_string

    mov     rdi, password
    call    string_length

    ;print empty hangman
    xor     rcx, rcx
    xor     r9, r9
    call    hangman_art

    ; fix guess variable
    mov     rdi, password
    call    string_length
    mov     rdi, guess
    add     rdi, rax
    mov     byte[rdi-1], 10
    mov     byte[rdi], 0


; game loop
    .l1:
        mov     rdi, guess_msg
        call    print_string

        ; get guess word
        xor     rsi, rsi
        sub     rsp, 30
        mov     rdi, rsp
        mov     rdx, 30
        call    read_string
        mov     rdi, rax
        push    rdi
        call    print_string
        ;add     rsp,30

        pop     rdi
        cmp     byte[rdi+2], 0
        jne     .word
        mov     al, byte[rdi]
        call    check
        jmp     .hangman_art

        .word:
        call    check_word
        cmp     rax, 1
        je      .game_win
        add     rsp, 30

        .hangman_art:
        cmp     r8, 0
        je      .l4
        .l3:

        call    print_newline
        mov     rdi, guess
        call    print_string
        ;call    print_newline

        mov     rdi, guess
        mov     rsi, password
        call    string_equals
        cmp     rax, 1
        
        je      .game_win

    jmp .l1

    .l4:
        inc     r9
        mov     rcx, r9
        call    hangman_art
        cmp     r9, 6
        je      .game_lose
        jmp     .l3


    .game_lose:
        mov     rdi, lose_msg
        call    print_string
        call    exit

    .game_win:
        mov     rdi, win_msg
        call    print_string

    call    exit


        


check:

    xor     rcx, rcx
    xor     r8, r8
    .l1:
        mov     rdi, password
        cmp     al, byte[rdi+rcx]
        je      .pos
        cmp     byte[rdi+rcx], 0
        je      .end

    .l2:
        inc     rcx
    jmp     .l1

    .pos:
        inc     r8
        mov     rsi, guess
        add     rsi, rcx
        mov     byte[rsi], al

        jmp     .l2

    .end:
        ret

    .end2:
        mov     rax, 1
        ret

check_word:
    mov     rsi, password
    call    string_equals
    cmp     rax, 1
    je      .end

    .end2:
    xor     rax, rax
    xor     r8, r8
    ret

    .end:
    mov     rax, 1
    ret



;to-do
;see if i can fix the overflow thing when entering a string >30btyes
;fix the lib thing so i have my asm lib in git
;add "wrong" to hangman ascii art
;clean code
