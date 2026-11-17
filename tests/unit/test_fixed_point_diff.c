/*
 * Test differenziale: versione C (riferimento) contro versione NASM.
 * Le due devono dare risultati identici bit per bit su:
 *   1. golden vector: tutte le coppie di un insieme di casi limite
 *      (zero, +-1 LSB, +-1.0, estremi del range, valori vicini all'overflow);
 *   2. input pseudo-casuali da un seed fisso, quindi riproducibili.
 * Il generatore e' un LCG scritto qui, non rand(): rand() cambia tra
 * librerie C diverse, questo no.
 */
#include "fixed_point.h"
#include "test.h"

#define RANDOM_PAIRS 100000

static const q32 golden[] = {
    0, 1, -1, 2, -2,
    Q32_ONE, -Q32_ONE, Q32_ONE / 2, -Q32_ONE / 2, 3 * Q32_ONE,
    256 * Q32_ONE, -256 * Q32_ONE,           /* 256 * 256 va in overflow */
    32767, -32768, 32768, -32769,            /* limiti di q32_from_int */
    Q32_MAX, Q32_MIN, Q32_MAX - 1, Q32_MIN + 1,
    123456789, -987654321,
};

static uint32_t lcg_state = 0x1201u;

static int32_t next_random(void)
{
    lcg_state = lcg_state * 1664525u + 1013904223u;
    return (int32_t)lcg_state;
}

static void check_pair(q32 a, q32 b)
{
    CHECK_EQ(q32_add_asm(a, b), q32_add(a, b));
    CHECK_EQ(q32_sub_asm(a, b), q32_sub(a, b));
    CHECK_EQ(q32_mul_asm(a, b), q32_mul(a, b));
    CHECK_EQ(q32_div_asm(a, b), q32_div(a, b));
}

int main(void)
{
    const int n = (int)(sizeof golden / sizeof golden[0]);

    for (int i = 0; i < n; i++) {
        CHECK_EQ(q32_from_int_asm(golden[i]), q32_from_int(golden[i]));
        for (int j = 0; j < n; j++) {
            check_pair(golden[i], golden[j]);
        }
    }

    for (int k = 0; k < RANDOM_PAIRS; k++) {
        q32 a = next_random();
        q32 b = next_random() >> (k % 24);   /* divisori anche piccoli */
        CHECK_EQ(q32_from_int_asm(a >> 15), q32_from_int(a >> 15));
        check_pair(a, b);
    }

    TEST_MAIN_END();
}
