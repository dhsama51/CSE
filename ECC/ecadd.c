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

void ECADDAfAf(unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int x2[], unsigned int y2[], int is_inf2, unsigned int x3[], unsigned int y3[], int* is_inf3) {
    unsigned int x1c[8] = { 0, }, y1c[8] = { 0, }, is_inf1c = 0;
    memcpy(x1c, x1, 32); memcpy(y1c, y1, 32); is_inf1c = is_inf1;
    unsigned int x2c[8] = { 0, }, y2c[8] = { 0, }, is_inf2c = 0;
    memcpy(x2c, x2, 32); memcpy(y2c, y2, 32); is_inf2c = is_inf2;
    memset(x3, 0, 32); memset(y3, 0, 32); *is_inf3 = 0;
    if (is_inf1c && is_inf2c) {
        *is_inf3 = 1;
        return;
    }
    else if (is_inf1c) {
        memcpy(x3, x2c, 32); memcpy(y3, y2c, 32);
        return;
    }
    else if (is_inf2c) {
        memcpy(x3, x1c, 32); memcpy(y3, y1c, 32);
        return;
    }
    else if (bigIntCmp(x1, x2, 8) == 0 && bigIntCmp(y1, y2, 8) != 0) {
        *is_inf3 = 1;
        return;
    }
    *is_inf3 = 0;

    unsigned int temp16[16] = { 0, };
    unsigned int temp8[8] = { 0, };

    unsigned int y2y1[8] = { 0, }; int borrow1;
    FpMinus(y2c, y1c, &borrow1, y2y1, 8);
    unsigned int x2x1[8] = { 0, }; int borrow2;
    FpMinus(x2c, x1c, &borrow2, x2x1, 8);

    unsigned int x2x1inv[8] = { 0, };
    bigIntInvBin(x2x1, x2x1inv, 8);
    bigIntMulPS(y2y1, x2x1inv, temp16);
    unsigned int lambda[8] = { 0, };
    FpReduct(temp16, lambda, 8);

    unsigned int lambda2[8] = { 0, };
    bigIntSquare(lambda, temp16, 8);
    FpReduct(temp16, lambda2, 8);

    int dummy = 0;
    FpMinus(lambda2, x1c, &dummy, x3, 8);
    FpMinus(x3, x2c, &dummy, x3, 8);

    FpMinus(x1c, x3, &dummy, temp8, 8);
    bigIntMulPS(lambda, temp8, temp16);
    FpReduct(temp16, temp8, 8);
    FpMinus(temp8, y1c, &dummy, y3, 8);
}

void ECADDJaAf(unsigned int X1[], unsigned int Y1[], unsigned int Z1[], unsigned int x2[], unsigned int y2[], int is_inf2, unsigned int X3[], unsigned int Y3[], unsigned int Z3[]) {
    unsigned int X1c[8] = { 0, }, Y1c[8] = { 0, }, Z1c[8] = { 0, };
    memcpy(X1c, X1, 32); memcpy(Y1c, Y1, 32); memcpy(Z1c, Z1, 32);
    memset(X3, 0, 32); memset(Y3, 0, 32); memset(Z3, 0, 32);
    unsigned int zero8[8] = { 0, };
    if (is_inf2) {
        memcpy(X3, X1c, 32); memcpy(Y3, Y1c, 32); memcpy(Z3, Z1c, 32);
        return;
    }
    if (bigIntCmp(Z1c, zero8, 8) == 0) {
        memcpy(X3, x2, 32); memcpy(Y3, y2, 32); Z3[0] = 1;
        return;
    }

    unsigned int temp16[16] = { 0, };
    unsigned int T1[8] = { 0, };
    unsigned int T2[8] = { 0, };
    unsigned int T3[8] = { 0, };
    unsigned int T4[8] = { 0, };
    int dummy = 0;

    bigIntSquare(Z1c, temp16, 8);
    FpReduct(temp16, T1, 8);

    bigIntMulPS(Z1c, T1, temp16);
    FpReduct(temp16, T2, 8);

    bigIntMulPS(x2, T1, temp16);
    FpReduct(temp16, T1, 8);

    bigIntMulPS(y2, T2, temp16);
    FpReduct(temp16, T2, 8);

    FpMinus(T1, X1c, &dummy, T1, 8);
    FpMinus(T2, Y1c, &dummy, T2, 8);

    if (bigIntCmp(T1, zero8, 8) == 0) {
        if (bigIntCmp(T2, zero8, 8) == 0) {
            unsigned one8[8] = { 1, 0, };
            ECDBLJa(x2, y2, one8, X3, Y3, Z3);
            return;
        }
        else {
            X3[0]++; Y3[0]++;
            return;
        }
    }
    bigIntMulPS(Z1c, T1, temp16);
    FpReduct(temp16, Z3, 8);

    bigIntSquare(T1, temp16, 8);
    FpReduct(temp16, T3, 8);

    bigIntMulPS(T3, T1, temp16);
    FpReduct(temp16, T4, 8);

    bigIntMulPS(T3, X1c, temp16);
    FpReduct(temp16, T3, 8);

    FpPlus(T3, T3, &dummy, T1, 8);

    bigIntSquare(T2, temp16, 8);
    FpReduct(temp16, X3, 8);

    FpMinus(X3, T1, &dummy, X3, 8);
    FpMinus(X3, T4, &dummy, X3, 8);
    FpMinus(T3, X3, &dummy, T3, 8);

    bigIntMulPS(T3, T2, temp16);
    FpReduct(temp16, T3, 8);

    bigIntMulPS(T4, Y1c, temp16);
    FpReduct(temp16, T4, 8);

    FpMinus(T3, T4, &dummy, Y3, 8);
}

void ECADDJaJa(unsigned int X1[], unsigned int Y1[], unsigned int Z1[], unsigned int X2[], unsigned int Y2[], unsigned int Z2[], unsigned int X3[], unsigned int Y3[], unsigned int Z3[]) {
    unsigned int zero8[8] = { 0, };
    if (bigIntCmp(Z1, zero8, 8) == 0) {
        memcpy(X3, X2, 32); memcpy(Y3, Y2, 32); memcpy(Z3, Z2, 32);
        return;
    }
    if (bigIntCmp(Z2, zero8, 8) == 0) {
        memcpy(X3, X1, 32); memcpy(Y3, Y1, 32); memcpy(Z3, Z1, 32);
        return;
    }
    unsigned int X1c[8], Y1c[8], Z1c[8], X2c[8], Y2c[8], Z2c[8];
    memcpy(X1c, X1, 32); memcpy(Y1c, Y1, 32); memcpy(Z1c, Z1, 32);
    memcpy(X2c, X2, 32); memcpy(Y2c, Y2, 32); memcpy(Z2c, Z2, 32);
    unsigned int X3c[8] = { 0, }, Y3c[8] = { 0, }, Z3c[8] = { 0, };

    unsigned int temp8[8] = { 0, };
    unsigned int temp16[16] = { 0, };

    unsigned int Z1_2[8] = { 0, };
    bigIntSquare(Z1c, temp16, 8);
    FpReduct(temp16, Z1_2, 8);
    unsigned int Z1_3[8] = { 0, };
    bigIntMulPS(Z1_2, Z1c, temp16);
    FpReduct(temp16, Z1_3, 8);

    unsigned int Z2_2[8] = { 0, };
    bigIntSquare(Z2c, temp16, 8);
    FpReduct(temp16, Z2_2, 8);
    unsigned int Z2_3[8] = { 0, };
    bigIntMulPS(Z2_2, Z2c, temp16);
    FpReduct(temp16, Z2_3, 8);

    unsigned int U1[8] = { 0, };
    bigIntMulPS(X1c, Z2_2, temp16);
    FpReduct(temp16, U1, 8);
    unsigned int U2[8] = { 0, };
    bigIntMulPS(X2c, Z1_2, temp16);
    FpReduct(temp16, U2, 8);

    unsigned int S1[8] = { 0, };
    bigIntMulPS(Y1c, Z2_3, temp16);
    FpReduct(temp16, S1, 8);
    unsigned int S2[8] = { 0, };
    bigIntMulPS(Y2c, Z1_3, temp16);
    FpReduct(temp16, S2, 8);

    if (bigIntCmp(U1, U2, 8) == 0) {
        if (bigIntCmp(S1, S2, 8) == 0) {
            ECDBLJa(X1c, Y1c, Z1c, X3, Y3, Z3);
            return;
        }
        else {
            memset(X3, 0, 32); memset(Y3, 0, 32); memset(Z3, 0, 32);
            X3[0] = 1; Y3[0] = 1;
            return;
        }
    }

    int dummy = 0;
    unsigned int H[8] = { 0, };
    FpMinus(U2, U1, &dummy, H, 8);
    unsigned int R[8] = { 0, };
    FpMinus(S2, S1, &dummy, R, 8);

    unsigned int R_2[8] = { 0, };
    bigIntMulPS(R, R, temp16);
    FpReduct(temp16, R_2, 8);

    unsigned int R_3[8] = { 0, };
    bigIntMulPS(R_2, R, temp16);
    FpReduct(temp16, R_3, 8);

    unsigned int H_2[8] = { 0, };
    bigIntMulPS(H, H, temp16);
    FpReduct(temp16, H_2, 8);

    unsigned int H_3[8] = { 0, };
    bigIntMulPS(H_2, H, temp16);
    FpReduct(temp16, H_3, 8);

    FpMinus(R_2, H_3, &dummy, X3c, 8);
    bigIntMulPS(U1, H_2, temp16);
    FpReduct(temp16, temp8, 8);
    FpMinus(X3c, temp8, &dummy, X3c, 8);
    FpMinus(X3c, temp8, &dummy, X3c, 8);

    bigIntMulPS(U1, H_2, temp16);
    FpReduct(temp16, Y3c, 8);
    FpMinus(Y3c, X3c, &dummy, Y3c, 8);
    bigIntMulPS(Y3c, R, temp16);
    FpReduct(temp16, Y3c, 8);

    bigIntMulPS(S1, H_3, temp16);
    FpReduct(temp16, temp8, 8);

    FpMinus(Y3c, temp8, &dummy, Y3c, 8);

    bigIntMulPS(Z1c, Z2c, temp16);
    FpReduct(temp16, Z3c, 8);
    bigIntMulPS(Z3c, H, temp16);
    FpReduct(temp16, Z3c, 8);

    memcpy(X3, X3c, 32); memcpy(Y3, Y3c, 32); memcpy(Z3, Z3c, 32);
}
