#ifndef _MULTIPLY_H
#define _MULTIPLY_H

#include "common.h"

/**
 * Double-precision multiplication
 */
#if defined(HAVE_UINT128)

#define DP_MULT(a,b,ol,oh) do { \
    __uint128_t pr;             \
    pr = (__uint128_t)(a)*(b);  \
    ol = (uint64_t)pr;          \
    oh = (uint64_t)(pr >> 64);  \
    } while (0)

#elif defined(_MSC_VER) && defined(_WIN64)

#include <windows.h>
#define DP_MULT(a,b,ol,oh) do { ol = UnsignedMultiply128(a,b,&oh); } while (0)

#else

uint64_t static inline dp_mult_128_32(uint64_t a, uint64_t b, uint64_t *oh);

uint64_t static inline dp_mult_128_32(uint64_t a, uint64_t b, uint64_t *oh)
{
    uint32_t al = (uint32_t) a;
    uint32_t ah = (uint32_t)(a >> 32);
    uint32_t bl = (uint32_t) b;
    uint32_t bh = (uint32_t)(b >> 32);

    uint64_t sum0, sum1a, sum1b, sum2, sum3;

    sum0 = (uint64_t)al*bl;
    sum1a = (uint64_t)al*bh;
    sum1b = (uint64_t)ah*bl;
    sum2 = (uint64_t)ah*bh;

    sum1a += sum0 >> 32;
    sum1b += sum1a;
    sum3 = sum1b < sum1a;
    sum2 += sum1b >> 32;
    sum3 += sum2 >> 32;

    *oh = (sum3 << 32) + (uint32_t)sum2;
    return (sum1b << 32) + (uint32_t)sum0;
}

#define DP_MULT(a,b,ol,oh) do { ol = dp_mult_128_32(a,b,&oh); } while (0)

#endif

/*
 * Square an integer a[].
 *
 * @param t     Location where the result is stored
 * @param a     Integer (array) that will be squared
 * @param words Size of the integer a[], in 64-bit words. The integer t[] is
 *              twice that size.
 */
void square(uint64_t *t, const uint64_t *a, size_t words);

/*
 * Multiply an integer a[] by a 128-bit scalar, and add the result to integer t[].
 *
 * @param t          Integer (array) where the result of the multiplication is added to
 * @param scratchpad Temporary space, with t_words+a_words 64-bit words
 * @param a          Integer (array) to multiply by a scalar
 * @param b0         Lower 64 bits of the scalar
 * @param b1         Higher 64 bits of the scalar
 * @param t_words    Size of integer t[], in 64-bit words. It must be at least a_words+2
 * @param a_words    Size of integer a[], in 64-bit words
 */
void addmul128(uint64_t *t, uint64_t *scratchpad, const uint64_t * a, uint64_t b0, uint64_t b1, size_t t_words, size_t a_words);
#endif
