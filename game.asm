;%include "lib.inc"
%include "mylib.inc"
%include "hangmanart.inc"

%define word_len 30

global _start

section .data
ground:      db        "----------------------------------------------------------------------------------",10,0
start_msg:   db        "Hangman! type a word to guess: ",10,0
guess_msg:   db        "Guess a letter or whole word: ",0
win_msg:     db        "You win!!",10,0
lose_msg:    db        "You lose :(",10,0

password:       times word_len db 0
guess:          times word_len db '_'

section .text

_start:
    mov     rdi, ground
    call    print_string
    mov     rdi, start_msg
    call    print_string

    ; get password
    xor     rsi, rsi
    mov     rdi, rsp
    mov     rdi, password
    mov     rdx, word_len
    call    read_string

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
        call    read_ch

        call    check

        cmp     r8, 0
        je      .l4
        .l3:

        call    print_newline
        mov     rdi, guess
        call    print_string
        call    print_newline

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

;to-do
;see if i can fix the overflow thing when entering a string >30btyes
; fix the double printing of guess_msg
; be able to guess thw whole word, change read_char for read string
