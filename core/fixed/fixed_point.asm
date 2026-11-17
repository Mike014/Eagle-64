; Fixed-point Q16.16 in NASM, Windows x64 ABI.
;
; Windows x64 ABI (quello che serve qui):
;   - 1° argomento in ecx, 2° in edx (32 bit, perché q32 è int32)
;   - valore di ritorno in eax
;   - rax, rcx, rdx, r8-r11 sono "volatili": li possiamo sporcare
;     senza salvarli. Non tocchiamo altri registri né lo stack,
;     quindi sono funzioni "leaf" e non serve prologo/epilogo.
;
; Stessa logica della versione C (fixed_point.c): il calcolo si fa
; a 64 bit e poi si satura nel range int32. I risultati devono essere
; identici bit per bit, lo verificano i test differenziali.

BITS 64
default rel
section .text

global q32_from_int_asm
global q32_add_asm
global q32_sub_asm
global q32_mul_asm
global q32_div_asm

; Satura rax (int64) nel range int32 e ritorna (risultato in eax).
; cmovg/cmovl = "copia se maggiore/minore", senza salti.
%macro SATURATE_AND_RET 0
    mov     rdx, 0x7FFFFFFF 
    cmp     rax, rdx  
    cmovg   rax, rdx
    mov     rdx, -0x80000000
    cmp     rax, rdx
    cmovl   rax, rdx
    ret 
%endmacro

; q32 q32_from_int_asm(int32_t x)
q32_from_int_asm:
    movsxd  rax, ecx 
    shl     rax, 16
    SATURATE_AND_RET 

; q32 q32_add_asm(q32 a, q32 b)
q32_add_asm:
    movsxd rax, ecx
    movsxd rdx, edx
    add    rax, rdx                 ; a 64 bit non può andare in overflow
    SATURATE_AND_RET

; q32 q32_sub_asm(q32 a, q32 b)
q32_sub_asm:
    movsxd rax, ecx
    movsxd rdx, edx
    sub    rax, rdx
    SATURATE_AND_RET

; q32 q32_mul_asm(q32 a, q32 b)
q32_mul_asm:
    movsxd rax, ecx
    movsxd rdx, edx
    imul   rax, rdx                 ; prodotto a 64 bit (scala B-32)
    sar    rax, 16                  ; torna a B-16, arrotonda verso -inf come il C
    SATURATE_AND_RET

; q32 q32_div_asm(q32 a, q32 b)
q32_div_asm:
    test   edx, edx
    jnz    .nonzero
    ; divisione per zero: satura con il segno del dividendo
    mov    eax, 0x7FFFFFFF
    test   ecx, ecx
    jns    .done 
    mov    eax, 0x80000000
.done: 
    ret  
.nonzero:
    movsxd r8, edx 
    movsxd rax, ecx
    shl    rax, 16 
    cqo
    idiv   r8
    SATURATE_AND_RET
    
