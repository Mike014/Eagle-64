/*
 * Fixed-point Q16.16: un int32 che vale  intero / 65536.
 *   16 bit interi con segno + 16 bit frazionari.
 *   Range [-32768.0, +32767.99998], risoluzione 1/65536.
 *
 * Origine AGC: niente floating point, parole a 15 bit in complemento a uno
 * usate come frazioni in [-1, +1]. Ogni variabile aveva la sua scala B-n,
 * ricordata solo nei commenti (es. FULLAPS DEC 5050 B-16 = massa in 2^16 kg).
 * Eagle-64 usa una sola scala fissa (sempre B-16), complemento a due,
 * e in caso di overflow satura invece di "avvolgersi".
 */

// include guard: se due file includono questo header,
// il contenuto viene comunque letto una volta sola. 
#ifndef EAGLE_FIXED_POINT_H
#define EAGLE_FIXED_POINT_H

#include <stdint.h>

typedef int32_t q32; // un intero con segno di esattamente 32 bit 

#define Q32_FRAC_BITS 16 // quanti bit sono frazionari
#define Q32_ONE ((q32)(1 << Q32_FRAC_BITS)) 
#define Q32_MAX ((q32)INT32_MAX)
#define Q32_MIN ((q32)INT32_MIN)

// funzioni senza implementazione
q32 q32_from_int(int32_t x);
q32 q32_add(q32 a, q32 b);
q32 q32_sub(q32 a, q32 b);
q32 q32_mul(q32 a, q32 b);
q32 q32_div(q32 a, q32 b);
q32 q32_from_int_asm(int32_t x);
q32 q32_add_asm(q32 a, q32 b);
q32 q32_sub_asm(q32 a, q32 b);
q32 q32_mul_asm(q32 a, q32 b);
q32 q32_div_asm(q32 a, q32 b);

#endif
