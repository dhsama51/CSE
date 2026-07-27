#include "bigIntPlusMinus.h"
#include "bigIntMulSqrRed.h"
#include "bigIntInv.h"
#include "bigIntGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rightShiftNbit(unsigned int a[], int n, int len) {
    if (n > 32) {
        for (int i = 0;i < 8 - (n / 32);i += 1) {
            a[i] = a[i + (n / 32)];
        }
        memset(a + len - (n / 32), 0, n / 8);
    }

    if (n % 32 != 0) {
        for (int i = 0;i < len - 1;i++) {
            a[i] = (a[i] >> (n % 32)) | (a[i + 1] << (32 - (n % 32)));
        }
        a[len - 1] >>= n % 32;
    }
}

void leftshiftNbit(unsigned int a[], int n, int len) {
    if (n > 32) {
        for (int i = 7;i >= (n / 32);i--) {
            a[i] = a[i - (n / 32)];
        }
        memset(a, 0, n / 8);
    }
    if (n % 32 != 0) {
        for (int i = len - 1;i >= 1;i--) {
            a[i] = (a[i] << (n % 32)) | (a[i - 1] >> (32 - (n % 32)));
        }
        a[0] <<= n % 32;
    }
}

void bigIntInvBin(unsigned int a[], unsigned c[], int len) {
    unsigned int u[8] = { 0, };
    unsigned int v[8] = { 0, };
    memcpy(u, a, len * 4);
    memcpy(v, Fp[0], len * 4);

    unsigned int x1[8] = { 0, }; x1[0] += 1;
    unsigned int x2[8] = { 0, };

    unsigned int zero[8] = { 0, };
    unsigned int one[8] = { 0, }; one[0] += 1;
    while (bigIntCmp(u, one, len) != 0 && bigIntCmp(v, one, len) != 0) {
        while (!(u[0] & 1)) {
            rightShiftNbit(u, 1, 8);
            if (!(x1[0] & 1)) rightShiftNbit(x1, 1, 8);
            else {
                int carry = 0;
                bigIntPlus(x1, Fp[0], &carry, x1, 8);
                rightShiftNbit(x1, 1, 8);
                x1[7] += (carry << 31);
            }
        }

        while (!(v[0] & 1)) {
            rightShiftNbit(v, 1, 8);
            if (!(x2[0] & 1)) rightShiftNbit(x2, 1, 8);
            else {
                int carry = 0;
                bigIntPlus(x2, Fp[0], &carry, x2, 8);
                rightShiftNbit(x2, 1, 8);
                x2[7] += (carry << 31);
            }
        }

        if (bigIntCmp(u, v, 8) >= 0) {
            int borrow = 0;
            bigIntMinus(u, v, &borrow, u, 8);
            FpMinus(x1, x2, &borrow, x1, 8);
        }
        else {
            int borrow = 0;
            bigIntMinus(v, u, &borrow, v, 8);
            FpMinus(x2, x1, &borrow, x2, 8);
        }
    }

    int carry = 0;
    if (bigIntCmp(u, one, 8) == 0) FpPlus(x1, zero, &carry, c, 8);
    else FpPlus(x2, zero, &carry, c, 8);
}

void bigIntInvFermat(unsigned int z[], unsigned int t[], int len) {
    unsigned int temp8[8] = { 0, };
    unsigned int temp16[16] = { 0, };

    unsigned int z3[8] = { 0, };
    bigIntSquare(z, temp16, len);
    FpReduct(temp16, temp8, len);
    bigIntMulKaratsuba(temp8, z, 8, temp16);
    FpReduct(temp16, z3, len);

    unsigned int z15[8] = { 0, };
    memcpy(temp8, z3, len * 4);
    for (int i = 0;i < 2;i++) {
        bigIntSquare(temp8, temp16, len);
        FpReduct(temp16, temp8, len);
    }
    bigIntMulKaratsuba(temp8, z3, len, temp16);
    FpReduct(temp16, z15, len);

    unsigned int t0[8] = { 0, };
    memcpy(temp8, z15, len * 4);
    for (int i = 0;i < 2;i++) {
        bigIntSquare(temp8, temp16, len);
        FpReduct(temp16, temp8, len);
    }
    bigIntMulKaratsuba(temp8, z3, len, temp16);
    FpReduct(temp16, t0, len);

    unsigned int t1[8] = { 0, };
    memcpy(temp8, t0, len * 4);
    for (int i = 0;i < 6;i++) {
        bigIntSquare(temp8, temp16, len);
        FpReduct(temp16, temp8, len);
    }
    bigIntMulKaratsuba(temp8, t0, 8, temp16);
    FpReduct(temp16, t1, len);

    unsigned int t2[8] = { 0, };
    memcpy(temp8, t1, len * 4);
    for (int i = 0;i < 12;i++) {
        bigIntSquare(temp8, temp16, len);
        FpReduct(temp16, temp8, len);
    }
    bigIntMulKaratsuba(temp8, t1, 8, temp16);
    FpReduct(temp16, temp8, len);
    for (int i = 0;i < 6;i++) {
        bigIntSquare(temp8, temp16, len);
        FpReduct(temp16, temp8, len);
    }
    bigIntMulKaratsuba(temp8, t0, 8, temp16);
    FpReduct(temp16, t2, len);

    unsigned int t3[8] = { 0, };
    memcpy(temp8, t2, len * 4);
    for (int i = 0;i < 2;i++) {
        bigIntSquare(temp8, temp16, len);
        FpReduct(temp16, temp8, len);
    }
    bigIntMulKaratsuba(temp8, z3, len, temp16);
    FpReduct(temp16, t3, len);

    unsigned int t4[8] = { 0, };
    memcpy(temp8, t3, len * 4);
    for (int i = 0;i < 32;i++) {
        bigIntSquare(temp8, temp16, len);
        FpReduct(temp16, temp8, len);
    }
    bigIntMulKaratsuba(temp8, z, len, temp16);
    FpReduct(temp16, temp8, len);
    for (int i = 0;i < 96;i++) {
        bigIntSquare(temp8, temp16, len);
        FpReduct(temp16, temp8, len);
    }
    memcpy(t4, temp8, len * 4);

    unsigned int t5[8] = { 0, };
    memcpy(temp8, t4, len * 4);
    for (int i = 0;i < 32;i++) {
        bigIntSquare(temp8, temp16, len);
        FpReduct(temp16, temp8, len);
    }
    bigIntMulKaratsuba(temp8, t3, len, temp16);
    FpReduct(temp16, temp8, len);
    for (int i = 0;i < 32;i++) {
        bigIntSquare(temp8, temp16, len);
        FpReduct(temp16, temp8, len);
    }
    bigIntMulKaratsuba(temp8, t3, len, temp16);
    FpReduct(temp16, t5, len);

    memcpy(temp8, t5, len * 4);
    for (int i = 0;i < 30;i++) {
        bigIntSquare(temp8, temp16, len);
        FpReduct(temp16, temp8, len);
    }
    bigIntMulKaratsuba(temp8, t2, len, temp16);
    FpReduct(temp16, temp8, len);
    for (int i = 0;i < 2;i++) {
        bigIntSquare(temp8, temp16, len);
        FpReduct(temp16, temp8, len);
    }
    bigIntMulKaratsuba(temp8, z, len, temp16);
    FpReduct(temp16, t, len);
}