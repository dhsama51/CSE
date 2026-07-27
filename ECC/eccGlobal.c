#include "bigIntPlusMinus.h"
#include "bigIntMulSqrRed.h"
#include "bigIntInv.h"
#include "bigIntGlobal.h"
#include "eccGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Ja2Af(unsigned int Jax[], unsigned int Jay[], unsigned int Jaz[], unsigned int Afx[], unsigned int Afy[], int* is_inf) {
    memset(Afx, 0, 32); memset(Afy, 0, 32); *is_inf = 0;
    unsigned int zero8[8] = { 0, };
    if (bigIntCmp(Jaz, zero8, 8) == 0) {
        *is_inf = 1;
        return;
    }

    unsigned int temp16[16] = { 0, };
    unsigned int Jaz_1[8] = { 0, };
    unsigned int Jaz_2[8] = { 0, };
    unsigned int Jaz_3[8] = { 0, };
    bigIntInvBin(Jaz, Jaz_1, 8);
    bigIntSquare(Jaz_1, temp16, 8);
    FpReduct(temp16, Jaz_2, 8);
    bigIntMulPS(Jaz_2, Jaz_1, temp16);
    FpReduct(temp16, Jaz_3, 8);

    bigIntMulPS(Jax, Jaz_2, temp16);
    FpReduct(temp16, Afx, 8);
    bigIntMulPS(Jay, Jaz_3, temp16);
    FpReduct(temp16, Afy, 8);
}