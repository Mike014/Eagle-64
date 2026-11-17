#include "fixed_point.h"
#include "test.h"

static void test_from_int(void)
{
    CHECK_EQ(q32_from_int(0), 0);
    CHECK_EQ(q32_from_int(1), Q32_ONE);
    CHECK_EQ(q32_from_int(-1), -Q32_ONE);
    CHECK_EQ(q32_from_int(32767), 32767 * Q32_ONE);
    CHECK_EQ(q32_from_int(-32768), Q32_MIN);
    CHECK_EQ(q32_from_int(32768), Q32_MAX);   /* fuori range: satura */
    CHECK_EQ(q32_from_int(-32769), Q32_MIN);
}

static void test_add(void)
{
    CHECK_EQ(q32_add(Q32_ONE, Q32_ONE), 2 * Q32_ONE);
    CHECK_EQ(q32_add(Q32_ONE, -Q32_ONE), 0);
    CHECK_EQ(q32_add(Q32_ONE / 2, Q32_ONE / 4), 3 * Q32_ONE / 4);
    CHECK_EQ(q32_add(Q32_MAX, 1), Q32_MAX);
    CHECK_EQ(q32_add(Q32_MAX, Q32_MAX), Q32_MAX);
    CHECK_EQ(q32_add(Q32_MIN, -1), Q32_MIN);
    CHECK_EQ(q32_add(Q32_MIN, Q32_MIN), Q32_MIN);
    CHECK_EQ(q32_add(Q32_MAX, Q32_MIN), -1);
}

static void test_sub(void)
{
    CHECK_EQ(q32_sub(3 * Q32_ONE, Q32_ONE), 2 * Q32_ONE);
    CHECK_EQ(q32_sub(0, Q32_ONE), -Q32_ONE);
    CHECK_EQ(q32_sub(Q32_MIN, 1), Q32_MIN);
    CHECK_EQ(q32_sub(Q32_MAX, -1), Q32_MAX);
    CHECK_EQ(q32_sub(0, Q32_MIN), Q32_MAX);   /* -MIN non sta in 32 bit */
    CHECK_EQ(q32_sub(Q32_MIN, Q32_MAX), Q32_MIN);
}

static void test_mul(void)
{
    CHECK_EQ(q32_mul(2 * Q32_ONE, 3 * Q32_ONE), 6 * Q32_ONE);
    CHECK_EQ(q32_mul(Q32_ONE / 2, Q32_ONE / 2), Q32_ONE / 4);
    CHECK_EQ(q32_mul(-2 * Q32_ONE, 3 * Q32_ONE), -6 * Q32_ONE);
    CHECK_EQ(q32_mul(-2 * Q32_ONE, -3 * Q32_ONE), 6 * Q32_ONE);
    CHECK_EQ(q32_mul(Q32_MAX, Q32_ONE), Q32_MAX);
    CHECK_EQ(q32_mul(12345, 0), 0);
    CHECK_EQ(q32_mul(256 * Q32_ONE, 256 * Q32_ONE), Q32_MAX);  /* 65536 */
    CHECK_EQ(q32_mul(-256 * Q32_ONE, 256 * Q32_ONE), Q32_MIN);
    CHECK_EQ(q32_mul(Q32_MIN, Q32_MIN), Q32_MAX);
    /* lo shift aritmetico arrotonda verso -inf: -1/65536 * 1/2 = -1 LSB */
    CHECK_EQ(q32_mul(-1, Q32_ONE / 2), -1);
}

static void test_div(void)
{
    CHECK_EQ(q32_div(6 * Q32_ONE, 3 * Q32_ONE), 2 * Q32_ONE);
    CHECK_EQ(q32_div(Q32_ONE, 2 * Q32_ONE), Q32_ONE / 2);
    CHECK_EQ(q32_div(-6 * Q32_ONE, 3 * Q32_ONE), -2 * Q32_ONE);
    CHECK_EQ(q32_div(-6 * Q32_ONE, -3 * Q32_ONE), 2 * Q32_ONE);
    CHECK_EQ(q32_div(0, Q32_ONE), 0);
    CHECK_EQ(q32_div(Q32_MAX, Q32_ONE / 2), Q32_MAX);   /* overflow */
    CHECK_EQ(q32_div(Q32_MIN, Q32_ONE / 2), Q32_MIN);
    CHECK_EQ(q32_div(Q32_MIN, -Q32_ONE), Q32_MAX);      /* -MIN satura */
    /* divisione per zero: satura col segno del dividendo */
    CHECK_EQ(q32_div(Q32_ONE, 0), Q32_MAX);
    CHECK_EQ(q32_div(0, 0), Q32_MAX);
    CHECK_EQ(q32_div(-Q32_ONE, 0), Q32_MIN);
}

int main(void)
{
    test_from_int();
    test_add();
    test_sub();
    test_mul();
    test_div();
    TEST_MAIN_END();
}
