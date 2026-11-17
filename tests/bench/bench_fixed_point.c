/*
 * Benchmark C vs NASM delle funzioni q32. Non e' un test: si lancia a
 * mano in Release e i risultati si annotano in docs/PLAN.md.
 *
 * Le funzioni stanno in un'altra unita' di compilazione, quindi il
 * compilatore non puo' inlinarle: si misura il costo di una chiamata
 * vera, uguale per C e NASM. Il risultato si accumula in una variabile
 * volatile perche' l'ottimizzatore non elimini il ciclo.
 */
#include <stdio.h>
#include <time.h>
#include "fixed_point.h"

#define N 1000      /* operandi */
#define ROUNDS 20000 /* passate sugli operandi: 20 milioni di chiamate */

typedef q32 (*binop)(q32, q32);

static q32 xs[N], ys[N];
static volatile q32 sink;

static double now_ns(void)
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec * 1e9 + (double)ts.tv_nsec;
}

static double bench(binop f)
{
    q32 acc = 0;
    double t0 = now_ns();
    for (int r = 0; r < ROUNDS; r++) {
        for (int i = 0; i < N; i++) {
            acc ^= f(xs[i], ys[i]);
        }
    }
    double t1 = now_ns();
    sink = acc;
    return (t1 - t0) / ((double)N * ROUNDS);
}

int main(void)
{
    uint32_t s = 0x1202u;
    for (int i = 0; i < N; i++) {
        s = s * 1664525u + 1013904223u; xs[i] = (q32)s;
        s = s * 1664525u + 1013904223u; ys[i] = (q32)(s | 1u) >> 8;
    }

    static const struct { const char *name; binop c, nasm; } ops[] = {
        { "add", q32_add, q32_add_asm },
        { "sub", q32_sub, q32_sub_asm },
        { "mul", q32_mul, q32_mul_asm },
        { "div", q32_div, q32_div_asm },
    };

    printf("op    C (ns/op)  NASM (ns/op)\n");
    for (int k = 0; k < 4; k++) {
        double c = bench(ops[k].c);
        double a = bench(ops[k].nasm);
        printf("%-4s  %9.3f  %12.3f\n", ops[k].name, c, a);
    }
    return 0;
}
