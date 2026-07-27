#include "bigIntPlusMinus.h"
#include "bigIntMulSqrRed.h"
#include "bigIntGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mulSplit(unsigned int a, unsigned int b, unsigned int* U, unsigned int* V) {
    unsigned int a_lo = a & 0xFFFF; unsigned int a_hi = a >> 16;
    unsigned int b_lo = b & 0xFFFF; unsigned int b_hi = b >> 16;

    unsigned int p0 = a_lo * b_lo;
    unsigned int p1 = a_lo * b_hi;
    unsigned int p2 = a_hi * b_lo;
    unsigned int p3 = a_hi * b_hi;

    unsigned int mid = p1 + p2;
    unsigned int carry_mid = mid < p1;

    *V = p0 + (mid << 16);
    int carry_v = *V < p0;

    *U = p3 + (mid >> 16) + carry_v + (carry_mid << 16);
}

void mulExpand(unsigned int a, unsigned int b, unsigned int* U, unsigned int* V) {
    *U = ((unsigned long long)a * (unsigned long long)b) >> 32;
    *V = ((unsigned long long)a * (unsigned long long)b);
}

void bigIntMulOS(unsigned int a[], unsigned int b[], unsigned int c[]) {
    unsigned int U = 0; unsigned int V = 0;
    memset(c, 0, 64);

    for (int i = 0;i < 8;i++) {
        U = 0;
        for (int j = 0;j < 8;j++) {
            unsigned int high = 0; unsigned int low = 0;
            mulExpand(a[i], b[j], &high, &low);

            V = c[i + j] + low;
            int carry = 0;
            carry += V < low;
            V += U;
            carry += V < U;

            U = high + carry;

            c[i + j] = V;
        }
        c[i + 8] = U;
    }
}

void bigIntMulPS(unsigned int a[], unsigned int b[], unsigned int c[]) {
    memset(c, 0, 64);
    unsigned int R0 = 0, R1 = 0, R2 = 0;
    for (int k = 0;k <= 14;k++) {
        for (int i = (k < 7) ? k : 7, j = k - i;i >= 0 && j <= 7;) {
            unsigned int high = 0, low = 0;
            mulSplit(a[i], b[j], &high, &low);

            int carry1 = 0;
            R0 += low;
            carry1 = R0 < low;

            int carry2 = 0;
            R1 += high;
            carry2 = R1 < high;
            R1 += carry1;
            carry2 += R1 < carry1;

            R2 += carry2;
            i--; j++;
        }
        c[k] = R0; R0 = R1; R1 = R2; R2 = 0;
    }
    c[15] = R0;
}

void bigIntMulKaratsuba(unsigned int a[], unsigned int b[], int size, unsigned int c[]) {
    if (size == 1) {
        mulSplit(*a, *b, c + 1, c);
        return;
    }
    int dummy = 0;;

    int half = size / 2;
    unsigned int* a_hi = a + half;
    unsigned int* a_lo = a;
    unsigned int* b_hi = b + half;
    unsigned int* b_lo = b;

    unsigned int L[8] = { 0, };
    unsigned int H[8] = { 0, };
    bigIntMulKaratsuba(a_lo, b_lo, half, L);
    bigIntMulKaratsuba(a_hi, b_hi, half, H);

    unsigned int diff_a[8] = { 0, };
    unsigned int diff_b[8] = { 0, };
    int signA = bigIntCmp(a_hi, a_lo, half) == 1;
    int signB = bigIntCmp(b_hi, b_lo, half) == 1;
    if (signA) bigIntMinus(a_hi, a_lo, &dummy, diff_a, half);
    else       bigIntMinus(a_lo, a_hi, &dummy, diff_a, half);
    if (signB) bigIntMinus(b_hi, b_lo, &dummy, diff_b, half);
    else       bigIntMinus(b_lo, b_hi, &dummy, diff_b, half);

    unsigned int M[16] = { 0, };
    bigIntMulKaratsuba(diff_a, diff_b, half, M);

    memset(c, 0, 8 * size);
    memcpy(c, L, size * 4);

    unsigned int temp[16] = { 0, };
    int carry1 = 0;
    bigIntPlus(H, L, &carry1, temp, size);

    int carry2 = 0; int borrow2 = 0;
    if (signA ^ signB) bigIntPlus(temp, M, &carry2, temp, size);
    else bigIntMinus(temp, M, &borrow2, temp, size);

    int carry3 = 0;
    bigIntPlus(c + half, temp, &carry3, c + half, size);

    unsigned int carry_total[16] = { 0, }; carry_total[half + size] = carry1 + carry2 - borrow2 + carry3;
    if (carry_total[half + size]) bigIntPlus(c, carry_total, &dummy, c, 2 * size);

    bigIntPlus(c + size, H, &dummy, c + size, size);

    return;
}

void bigIntSquare(unsigned int a[], unsigned int c[], int len) {
    unsigned int R0 = 0, R1 = 0, R2 = 0;
    for (int k = 0;k <= 2 * len - 2;k++) {
        for (int i = (k < (len - 1)) ? k : len - 1, j = k - i;j <= i;) {
            unsigned int U[2];
            mulSplit(a[i], a[j], U, U + 1);

            int carry = 0;
            if (i > j) {
                carry = U[0] >> 31;
                U[0] = (U[0] << 1) | (U[1] >> 31);
                U[1] <<= 1;
                R2 = R2 + carry;
            }
            unsigned int one = 1;
            bigIntPlus(&R0, U + 1, &carry, &R0, 1);
            if (carry) bigIntPlus(&R1, &one, &carry, &R1, 1);
            bigIntPlus(&R1, U, &carry, &R1, 1);
            if (carry) bigIntPlus(&R2, &one, &carry, &R2, 1);

            i--; j++;
        }
        c[k] = R0; R0 = R1; R1 = R2, R2 = 0;
    }
    c[2 * len - 1] = R0;
}

void FpReduct(unsigned int a[], unsigned int c[], int len) {
    unsigned int s[9][8] = {
        { a[0],  a[1],  a[2],  a[3],  a[4],  a[5],  a[6],  a[7] },
        { 0,     0,     0,     a[11], a[12], a[13], a[14], a[15] },
        { 0,     0,     0,     a[12], a[13], a[14], a[15], 0     },
        { a[8],  a[9],  a[10], 0,     0,     0,     a[14], a[15] },
        { a[9],  a[10], a[11], a[13], a[14], a[15], a[13], a[8]  },
        { a[11], a[12], a[13], 0,     0,     0,     a[8],  a[10] },
        { a[12], a[13], a[14], a[15], 0,     0,     a[9],  a[11] },
        { a[13], a[14], a[15], a[8],  a[9],  a[10], 0,     a[12] },
        { a[14], a[15], 0,     a[9],  a[10], a[11], 0,     a[13] }
    };

    int carry_sum = 0, carry = 0, borrow = 0;
    bigIntPlus(s[0], s[1], &carry, c, len); carry_sum += carry;
    bigIntPlus(c, s[1], &carry, c, len); carry_sum += carry;
    bigIntPlus(c, s[2], &carry, c, len); carry_sum += carry;
    bigIntPlus(c, s[2], &carry, c, len); carry_sum += carry;
    bigIntPlus(c, s[3], &carry, c, len); carry_sum += carry;
    bigIntPlus(c, s[4], &carry, c, len); carry_sum += carry;

    bigIntMinus(c, s[5], &borrow, c, len); carry_sum -= borrow;
    bigIntMinus(c, s[6], &borrow, c, len); carry_sum -= borrow;
    bigIntMinus(c, s[7], &borrow, c, len); carry_sum -= borrow;
    bigIntMinus(c, s[8], &borrow, c, len); carry_sum -= borrow;
    if (carry_sum > 0)
        bigIntMinus(c, Fp[carry_sum - 1], &borrow, c, 8);
    else if (carry_sum < 0)
        bigIntPlus(c, Fp[-1 - carry_sum], &carry, c, 8);

    int dummy = 0;
    if (bigIntCmp(c, Fp[0], 8) >= 0) bigIntMinus(c, Fp[0], &dummy, c, 8);
}