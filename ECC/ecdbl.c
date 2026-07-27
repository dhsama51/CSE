#include "bigIntPlusMinus.h"
#include "bigIntMulSqrRed.h"
#include "bigIntInv.h"
#include "bigIntGlobal.h"
#include "ecadd.h"
#include "ecdbl.h"
#include "eccGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ECDBLAf(unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int x3[], unsigned int y3[], int* is_inf3) {
    unsigned int x1c[8] = { 0, }, y1c[8] = { 0, }, is_inf1c = 0;
    memcpy(x1c, x1, 32); memcpy(y1c, y1, 32); is_inf1c = is_inf1;
    memset(x3, 0, 32); memset(y3, 0, 32); *is_inf3 = 0;

    unsigned int zero8[8] = { 0, };
    if (is_inf1c == 1 || bigIntCmp(y1, zero8, 32) == 0) {
        memset(x3, 0, 32); memset(y3, 0, 32); *is_inf3 = 1;
        return;
    }
    unsigned int temp16[16] = { 0, };
    unsigned int temp8[8] = { 0, };

    unsigned int high[8] = { 0, };
    bigIntSquare(x1c, temp16, 8);
    FpReduct(temp16, temp8, 8);
    int dummy = 0;

    unsigned int x2_2[8] = { 0 };
    unsigned int x2_3[8] = { 0 };
    FpPlus(temp8, temp8, &dummy, x2_2, 8);
    FpPlus(x2_2, temp8, &dummy, x2_3, 8);
    FpPlus(x2_3, Fp_a, &dummy, high, 8);

    unsigned int low[8] = { 0, };
    FpPlus(y1c, y1c, &dummy, temp8, 8);
    bigIntInvBin(temp8, low, 8);

    unsigned int lambda[8] = { 0, };
    bigIntMulPS(high, low, temp16);
    FpReduct(temp16, lambda, 8);

    unsigned int lambda2[8] = { 0, };
    bigIntSquare(lambda, temp16, 8);
    FpReduct(temp16, lambda2, 8);

    FpPlus(x1c, x1c, &dummy, temp8, 8);
    FpMinus(lambda2, temp8, &dummy, x3, 8);

    FpMinus(x1c, x3, &dummy, temp8, 8);
    bigIntMulPS(lambda, temp8, temp16);
    FpReduct(temp16, temp8, 8);
    FpMinus(temp8, y1c, &dummy, y3, 8);
}

void ECDBLJa(unsigned int X1[], unsigned int Y1[], unsigned int Z1[], unsigned int X3[], unsigned int Y3[], unsigned int Z3[]) {
    unsigned int X1c[8], Y1c[8], Z1c[8];
    memcpy(X1c, X1, 32); memcpy(Y1c, Y1, 32); memcpy(Z1c, Z1, 32);
    memset(X3, 0, 32); memset(Y3, 0, 32); memset(Z3, 0, 32);
    unsigned int zero8[8] = { 0, };
    if (bigIntCmp(Z1c, zero8, 8) == 0) {
        X3[0]++; Y3[0]++;
        return;
    }

    unsigned int temp8[8] = { 0, };
    unsigned int temp16[16] = { 0, };
    unsigned int T1[8] = { 0, };
    unsigned int T2[8] = { 0, };
    unsigned int T3[8] = { 0, };
    int dummy = 0;

    bigIntSquare(Z1c, temp16, 8);
    FpReduct(temp16, T1, 8);

    FpMinus(X1c, T1, &dummy, T2, 8);
    FpPlus(X1c, T1, &dummy, T1, 8);

    bigIntMulPS(T2, T1, temp16);
    FpReduct(temp16, T2, 8);

    FpPlus(T2, T2, &dummy, temp8, 8);
    FpPlus(T2, temp8, &dummy, T2, 8);

    FpPlus(Y1c, Y1c, &dummy, Y3, 8);

    bigIntMulPS(Y3, Z1c, temp16);
    FpReduct(temp16, Z3, 8);

    bigIntSquare(Y3, temp16, 8);
    FpReduct(temp16, Y3, 8);

    bigIntMulPS(Y3, X1c, temp16);
    FpReduct(temp16, T3, 8);

    bigIntSquare(Y3, temp16, 8);
    FpReduct(temp16, Y3, 8);

    if (Y3[0] & 1) {
        int carry = 0;
        bigIntPlus(Y3, Fp[0], &carry, Y3, 8);
        rightShiftNbit(Y3, 1, 8);
        Y3[7] |= (carry << 31);

    }
    else {
        rightShiftNbit(Y3, 1, 8);
    }

    bigIntSquare(T2, temp16, 8);
    FpReduct(temp16, X3, 8);

    FpPlus(T3, T3, &dummy, T1, 8);
    FpMinus(X3, T1, &dummy, X3, 8);
    FpMinus(T3, X3, &dummy, T1, 8);

    bigIntMulPS(T1, T2, temp16);
    FpReduct(temp16, T1, 8);

    FpMinus(T1, Y3, &dummy, Y3, 8);
}
