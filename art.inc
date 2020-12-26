
section .data
hangman_0:  db  "==========",10
            db  "  +---+  ",10
            db  "  |   |  ",10
            db  "      |  ",10
            db  "      |  ",10
            db  "      |  ",10
            db  "      |  ",10
            db  "==========",10,10,0

hangman_1:  db  "==========",10
            db  "  +---+  ",10
            db  "  |   |  ",10
            db  "  0   |  ",10
            db  "      |  ",10
            db  "      |  ",10
            db  "      |  ",10
            db  "==========",10
            db  "Wrong!",10,0

hangman_2:  db  "==========",10
            db  "  +---+  ",10
            db  "  |   |  ",10
            db  "  0   |  ",10
            db  "  |   |  ",10
            db  "      |  ",10
            db  "      |  ",10
            db  "==========",10
            db  "Wrong!",10,0

hangman_3:  db  "==========",10
            db  "  +---+  ",10
            db  "  |   |  ",10
            db  "  0   |  ",10
            db  " /|   |  ",10
            db  "      |  ",10
            db  "      |  ",10
            db  "==========",10
            db  "Wrong!",10,0

hangman_4:  db  "==========",10
            db  "  +---+  ",10
            db  "  |   |  ",10
            db  "  0   |  ",10
            db  " /|\  |  ",10
            db  "      |  ",10
            db  "      |  ",10
            db  "==========",10
            db  "Wrong!",10,0

hangman_5:  db  "==========",10
            db  "  +---+  ",10
            db  "  |   |  ",10
            db  "  0   |  ",10
            db  " /|\  |  ",10
            db  " /    |  ",10
            db  "      |  ",10
            db  "==========",10
            db  "Wrong!",10,0

hangman_6:  db  "==========",10
            db  "  +---+  ",10
            db  "  |   |  ",10
            db  "  0   |  ",10
            db  " /|\  |  ",10
            db  " / \  |  ",10
            db  "      |  ",10
            db  "==========",10
            db  "GAME OVER!",10,0


section .text

hangman_art:


    cmp     rcx, 0
    je      .hang0
    cmp     rcx, 1
    je      .hang1
    cmp     rcx, 2
    je      .hang2
    cmp     rcx, 3
    je      .hang3
    cmp     rcx, 4
    je      .hang4
    cmp     rcx, 5
    je      .hang5
    cmp     rcx, 6
    je      .hang6
    jmp     .end
   


    .hang0:
        mov     rdi, hangman_0
        call    print_string
        jmp    .end
    .hang1:
        mov     rdi, hangman_1
        call    print_string
        jmp    .end
    .hang2:
        mov     rdi, hangman_2
        call    print_string
        jmp    .end
    .hang3:
        mov     rdi, hangman_3
        call    print_string
        jmp    .end
    .hang4:
        mov     rdi, hangman_4
        call    print_string
        jmp    .end
    .hang5:
        mov     rdi, hangman_5
        call    print_string
        jmp    .end
    .hang6:
        mov     rdi, hangman_6
        jmp    print_string

    .end:
        xor     rax, rax
        ret
       
