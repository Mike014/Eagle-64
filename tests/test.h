/*
 * Mini-framework di test in C, senza dipendenze.
 * Ogni CHECK che fallisce stampa file:riga e incrementa test_failures;
 * TEST_MAIN_END restituisce 1 se c'e' almeno un fallimento, cosi' CTest
 * segna il test come fallito (CTest guarda solo l'exit code).
 */
#ifndef EAGLE_TEST_H
#define EAGLE_TEST_H

#include <stdio.h>

static int test_failures = 0;
static int test_checks = 0;

#define CHECK(cond)                                                     \
    do {                                                                \
        test_checks++;                                                  \
        if (!(cond)) {                                                  \
            test_failures++;                                            \
            printf("%s:%d: CHECK fallito: %s\n", __FILE__, __LINE__, #cond); \
        }                                                               \
    } while (0)

#define CHECK_EQ(actual, expected)                                      \
    do {                                                                \
        long long a_ = (long long)(actual);                             \
        long long e_ = (long long)(expected);                           \
        test_checks++;                                                  \
        if (a_ != e_) {                                                 \
            test_failures++;                                            \
            printf("%s:%d: %s = %lld, atteso %lld\n",                   \
                   __FILE__, __LINE__, #actual, a_, e_);                \
        }                                                               \
    } while (0)

#define TEST_MAIN_END()                                                 \
    do {                                                                \
        printf("%d check, %d falliti\n", test_checks, test_failures);   \
        return test_failures ? 1 : 0;                                   \
    } while (0)

#endif
