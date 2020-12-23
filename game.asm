;%include "lib.inc"
%include "mylib.inc"
%include "hangmanart.inc"

%define word_len 30

global _start

section .data
ground:      db        "----------------------------------------------------------------------------------",10,0
start_msg:   db        "Hangman! type a word to guess: ",10,0
guess_msg:   db        "Guess a letter or whole word: ",0
password:       times word_len db 0
guess:          times word_len db '_'

section .text

_start:
;    call    clear_screen
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
    call    hangman_art

    ; fix guess variable
    mov     rdi, password
    call    string_length
    mov     rdi, guess
    add     rdi, rax
    mov     byte[rdi-1], 0


; game loop
;    .l1:
        
        
        


    mov     rdi, password
    call    print_string
    mov     rdi, guess
    call    print_string





    call    exit



;to-do
;see if i can fix the overflow thing when entering a string >30btyes
;hangman logic
