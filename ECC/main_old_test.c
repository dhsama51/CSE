#include "bigIntIO.h"
#include "bigIntPlusMinus.h"
#include "bigIntMulSqrRed.h"
#include "bigIntInv.h"
#include "bigIntGlobal.h"
#include "ecadd.h"
#include "ecdbl.h"
#include "eccsm.h"
#include "eccGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ECADDAfAfTest() {
    char x1str[] = "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296";
    char y1str[] = "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5";
    int is_inf1 = 0;
    char x2str[] = "7cf27b188d034f7e8a52380304b51ac3c08969e277f21b35a60b48fc47669978";
    char y2str[] = "07775510db8ed040293d9ac69f7430dbba7dade63ce982299e04b79d227873d1";
    int is_inf2 = 0;

    unsigned int x1[8] = { 0, }, y1[8] = { 0, };
    for (int i = 7;i >= 0;i--) sscanf(x1str + 8 * (7 - i), " %08X", x1 + i);
    for (int i = 7;i >= 0;i--) sscanf(y1str + 8 * (7 - i), " %08X", y1 + i);

    unsigned int x2[8] = { 0, }, y2[8] = { 0, };
    for (int i = 7;i >= 0;i--) sscanf(x2str + 8 * (7 - i), " %08X", x2 + i);
    for (int i = 7;i >= 0;i--) sscanf(y2str + 8 * (7 - i), " %08X", y2 + i);

    unsigned int x3[8] = { 0, }, y3[8] = { 0, };
    int is_inf3 = 0;
    ECADDAfAf(x1, y1, is_inf1, x2, y2, is_inf2, x3, y3, &is_inf3);
    printf("x1: "); for (int i = 7;i >= 0;i--) printf("%08X ", x1[i]); printf("\n");
    printf("y1: "); for (int i = 7;i >= 0;i--) printf("%08X ", y1[i]); printf("\n");
    printf("x2: "); for (int i = 7;i >= 0;i--) printf("%08X ", x2[i]); printf("\n");
    printf("y2: "); for (int i = 7;i >= 0;i--) printf("%08X ", y2[i]); printf("\n");
    printf("x3: "); for (int i = 7;i >= 0;i--) printf("%08X ", x3[i]); printf("\n");
    printf("y3: "); for (int i = 7;i >= 0;i--) printf("%08X ", y3[i]); printf("\n");
}

void ECDBLAfTest() {
    char x1str[] = "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296";
    char y1str[] = "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5";

    unsigned int x1[8] = { 0, }, y1[8] = { 0, }; int is_inf1 = 0;
    for (int i = 7;i >= 0;i--) sscanf(x1str + 8 * (7 - i), " %08X", x1 + i);
    for (int i = 7;i >= 0;i--) sscanf(y1str + 8 * (7 - i), " %08X", y1 + i);

    unsigned int x3[8] = { 0, }, y3[8] = { 0, }; int is_inf3 = 0;
    ECDBLAf(x1, y1, is_inf1, x3, y3, &is_inf3);
    printf("x1: "); for (int i = 7;i >= 0;i--) printf("%08X ", x1[i]); printf("\n");
    printf("y1: "); for (int i = 7;i >= 0;i--) printf("%08X ", y1[i]); printf("\n");
    printf("x2: "); for (int i = 7;i >= 0;i--) printf("%08X ", x3[i]); printf("\n");
    printf("x2: "); for (int i = 7;i >= 0;i--) printf("%08X ", y3[i]); printf("\n");
}

void Ja2AfTest() {
    char Jaxstr[] = "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296";
    char Jaystr[] = "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5";
    unsigned int Jax[8] = { 0, };
    unsigned int Jay[8] = { 0, };
    unsigned int Jaz[8] = { 0, };
    for (int i = 7;i >= 0;i--) sscanf(Jaxstr + 8 * (7 - i), " %08X", Jax + i);
    for (int i = 7;i >= 0;i--) sscanf(Jaystr + 8 * (7 - i), " %08X", Jay + i);
    Jaz[0] += 1;

    unsigned int Afx[8] = { 0, };
    unsigned int Afy[8] = { 0, };
    int is_inf = 0;
    Ja2Af(Jax, Jay, Jaz, Afx, Afy, &is_inf);
    printf("Jax: "); for (int i = 7;i >= 0;i--) printf("%08X ", Jax[i]); printf("\n");
    printf("Jay: "); for (int i = 7;i >= 0;i--) printf("%08X ", Jay[i]); printf("\n");
    printf("Jaz: "); for (int i = 7;i >= 0;i--) printf("%08X ", Jaz[i]); printf("\n\n");
    printf("Afx: "); for (int i = 7;i >= 0;i--) printf("%08X ", Afx[i]); printf("\n");
    printf("Afy: "); for (int i = 7;i >= 0;i--) printf("%08X ", Afy[i]); printf("\n");
}

void ECDBLJaTest() {
    char x1str[] = "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296";
    char y1str[] = "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5";

    unsigned int X1[8] = { 0, }, Y1[8] = { 0, }, Z1[8] = { 0, };
    for (int i = 7;i >= 0;i--) sscanf(x1str + 8 * (7 - i), " %08X", X1 + i);
    for (int i = 7;i >= 0;i--) sscanf(y1str + 8 * (7 - i), " %08X", Y1 + i);
    Z1[0] += 1;

    unsigned int X3[8] = { 0, }, Y3[8] = { 0, }, Z3[8] = { 0, };
    unsigned int x3[8] = { 0, }, y3[8] = { 0, }; int is_inf = 0;
    ECDBLJa(X1, Y1, Z1, X3, Y3, Z3);
    printf("X1: "); for (int i = 7;i >= 0;i--) printf("%08X ", X1[i]); printf("\n");
    printf("Y1: "); for (int i = 7;i >= 0;i--) printf("%08X ", Y1[i]); printf("\n");
    printf("Z1: "); for (int i = 7;i >= 0;i--) printf("%08X ", Z1[i]); printf("\n");
    printf("\n");
    printf("X3: "); for (int i = 7;i >= 0;i--) printf("%08X ", X3[i]); printf("\n");
    printf("Y3: "); for (int i = 7;i >= 0;i--) printf("%08X ", Y3[i]); printf("\n");
    printf("Z3: "); for (int i = 7;i >= 0;i--) printf("%08X ", Z3[i]); printf("\n");
    Ja2Af(X3, Y3, Z3, x3, y3, &is_inf);
    printf("x3: "); for (int i = 7;i >= 0;i--) printf("%08X ", x3[i]); printf("\n");
    printf("y3: "); for (int i = 7;i >= 0;i--) printf("%08X ", y3[i]); printf("\n");
}

void ECADDJaAfTest() {
    char x1str[] = "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296";
    char y1str[] = "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5";

    char x2str[] = "7cf27b188d034f7e8a52380304b51ac3c08969e277f21b35a60b48fc47669978";
    char y2str[] = "07775510db8ed040293d9ac69f7430dbba7dade63ce982299e04b79d227873d1";

    unsigned int X1[8] = { 0, }, Y1[8] = { 0, }, Z1[8] = { 0, };
    for (int i = 7;i >= 0;i--) sscanf(x1str + 8 * (7 - i), " %08X", X1 + i);
    for (int i = 7;i >= 0;i--) sscanf(y1str + 8 * (7 - i), " %08X", Y1 + i);
    Z1[0] += 1;

    unsigned int x2[8] = { 0, }, y2[8] = { 0, }; int is_inf2 = 0;
    for (int i = 7;i >= 0;i--) sscanf(x2str + 8 * (7 - i), " %08X", x2 + i);
    for (int i = 7;i >= 0;i--) sscanf(y2str + 8 * (7 - i), " %08X", y2 + i);

    unsigned int X3[8] = { 0, }, Y3[8] = { 0, }, Z3[8] = { 0, };
    unsigned int x3[8] = { 0, }, y3[8] = { 0, }; int is_inf3 = 0;
    ECADDJaAf(X1, Y1, Z1, x2, y2, is_inf2, X3, Y3, Z3);
    printf("X1: "); for (int i = 7;i >= 0;i--) printf("%08X ", X1[i]); printf("\n");
    printf("Y1: "); for (int i = 7;i >= 0;i--) printf("%08X ", Y1[i]); printf("\n");
    printf("Z1: "); for (int i = 7;i >= 0;i--) printf("%08X ", Z1[i]); printf("\n");
    printf("x2: "); for (int i = 7;i >= 0;i--) printf("%08X ", x2[i]); printf("\n");
    printf("y2: "); for (int i = 7;i >= 0;i--) printf("%08X ", y2[i]); printf("\n");
    printf("\n");
    printf("X3: "); for (int i = 7;i >= 0;i--) printf("%08X ", X3[i]); printf("\n");
    printf("Y3: "); for (int i = 7;i >= 0;i--) printf("%08X ", Y3[i]); printf("\n");
    printf("Z3: "); for (int i = 7;i >= 0;i--) printf("%08X ", Z3[i]); printf("\n");
    Ja2Af(X3, Y3, Z3, x3, y3, &is_inf3);
    printf("x3: "); for (int i = 7;i >= 0;i--) printf("%08X ", x3[i]); printf("\n");
    printf("y3: "); for (int i = 7;i >= 0;i--) printf("%08X ", y3[i]); printf("\n");
}

void ECADDJaJaTest() {
    char x1str[] = "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296";
    char y1str[] = "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5";

    char x2str[] = "7cf27b188d034f7e8a52380304b51ac3c08969e277f21b35a60b48fc47669978";
    char y2str[] = "07775510db8ed040293d9ac69f7430dbba7dade63ce982299e04b79d227873d1";

    unsigned int X1[8] = { 0, }, Y1[8] = { 0, }, Z1[8] = { 0, };
    for (int i = 7;i >= 0;i--) sscanf(x1str + 8 * (7 - i), " %08X", X1 + i);
    for (int i = 7;i >= 0;i--) sscanf(y1str + 8 * (7 - i), " %08X", Y1 + i);
    Z1[0] = 1;

    unsigned int X2[8] = { 0, }, Y2[8] = { 0, }, Z2[8] = { 0, };
    for (int i = 7;i >= 0;i--) sscanf(x2str + 8 * (7 - i), " %08X", X2 + i);
    for (int i = 7;i >= 0;i--) sscanf(y2str + 8 * (7 - i), " %08X", Y2 + i);
    Z2[0] = 1;

    unsigned int X3[8] = { 0, }, Y3[8] = { 0, }, Z3[8] = { 0, };
    unsigned int x3[8] = { 0, }, y3[8] = { 0, }; int is_inf3 = 0;
    ECADDJaJa(X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3);
    printf("X1: "); for (int i = 7;i >= 0;i--) printf("%08X ", X1[i]); printf("\n");
    printf("Y1: "); for (int i = 7;i >= 0;i--) printf("%08X ", Y1[i]); printf("\n");
    printf("Z1: "); for (int i = 7;i >= 0;i--) printf("%08X ", Z1[i]); printf("\n");
    printf("X2: "); for (int i = 7;i >= 0;i--) printf("%08X ", X2[i]); printf("\n");
    printf("Y2: "); for (int i = 7;i >= 0;i--) printf("%08X ", Y2[i]); printf("\n");
    printf("Z2: "); for (int i = 7;i >= 0;i--) printf("%08X ", Z2[i]); printf("\n");
    printf("\n");
    printf("X3: "); for (int i = 7;i >= 0;i--) printf("%08X ", X3[i]); printf("\n");
    printf("Y3: "); for (int i = 7;i >= 0;i--) printf("%08X ", Y3[i]); printf("\n");
    printf("Z3: "); for (int i = 7;i >= 0;i--) printf("%08X ", Z3[i]); printf("\n");
    Ja2Af(X3, Y3, Z3, x3, y3, &is_inf3);
    printf("x3: "); for (int i = 7;i >= 0;i--) printf("%08X ", x3[i]); printf("\n");
    printf("y3: "); for (int i = 7;i >= 0;i--) printf("%08X ", y3[i]); printf("\n");
}

void ECCSMTest() {
    char kstr[] = "DDB7F11471AFC9F6B6D14865B568A7A2BA08EE995E4D9E0A18671BCA3933224B";
    char x1str[] = "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296";
    char y1str[] = "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5";

    unsigned int k[8] = { 0, };
    unsigned int x1[8] = { 0, };
    unsigned int y1[8] = { 0, };
    int is_inf1 = 0;
    for (int i = 7;i >= 0;i--) sscanf(x1str + 8 * (7 - i), " %08X", x1 + i);
    for (int i = 7;i >= 0;i--) sscanf(y1str + 8 * (7 - i), " %08X", y1 + i);
    for (int i = 7;i >= 0;i--) sscanf(kstr + 8 * (7 - i), " %08X", k + i);

    unsigned int X3[8] = { 0, };
    unsigned int Y3[8] = { 0, };
    unsigned int Z3[8] = { 0, };
    unsigned int x3[8] = { 0, };
    unsigned int y3[8] = { 0, };
    int is_inf3 = 0;

    ECCSMLtRJa(k, x1, y1, is_inf1, X3, Y3, Z3);
    ECCSMLtRAf(k, x1, y1, is_inf1, x3, y3, &is_inf3);
    ECCSMRtLAf(k, x1, y1, is_inf1, x3, y3, &is_inf3);

    /*char kNAF[258] = "";
    int l = int2NAF(k, kNAF);
    for (int i = 0;i < l;i++) printf("%d", kNAF[i]); printf("\n%d\n", l);
    ECCSMLtRNAF(k, x1, y1, is_inf1, X3, Y3, Z3);*/

    /*int w = 3;
    char kwNAF[258] = "";
    int l = int2wNAF(k, kwNAF, w);
    for (int i = 0;i < l;i++) printf("%d", kwNAF[i]); printf("\n%d\n", l);
    ECCSMLtRwNAF(k, x1, y1, is_inf1, X3, Y3, Z3, w);*/

    /*int w = 4;
    ECCSMSlideWin(k, x1, y1, is_inf1, X3, Y3, Z3, w);*/

    /*int w = 4;
    ECCSMFixedWin(k, x1, y1, is_inf1, X3, Y3, Z3, w);
    ECCSMFixedComb(k, x1, y1, is_inf1, X3, Y3, Z3, w);*/

    /*int w = 2;

    unsigned int x2[8] = { 0, }, y2[8] = { 0, };
    char x2str[] = "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296";
    char y2str[] = "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5";
    for (int i = 7;i >= 0;i--) sscanf(x2str + 8 * (7 - i), " %08X", x2 + i);
    for (int i = 7;i >= 0;i--) sscanf(y2str + 8 * (7 - i), " %08X", y2 + i);
    int is_inf2 = 0;

    unsigned int l[8] = { 0, };
    memset(k, 0, 32); memset(l, 0, 32);
    k[0] = 2, l[0] = 3;

    ECCMSMShamir(k, x1, y1, is_inf1, l, x2, y2, is_inf2, X3, Y3, Z3, w);*/

    printf("x1: "); for (int i = 7;i >= 0;i--) printf("%08X ", x1[i]); printf("\n");
    printf("y1: "); for (int i = 7;i >= 0;i--) printf("%08X ", y1[i]); printf("\n");
    /*printf("x2: "); for (int i = 7;i >= 0;i--) printf("%08X ", x2[i]); printf("\n");
    printf("y2: "); for (int i = 7;i >= 0;i--) printf("%08X ", y2[i]); printf("\n");*/
    printf("\n");
    printf("X3: "); for (int i = 7;i >= 0;i--) printf("%08X ", X3[i]); printf("\n");
    printf("Y3: "); for (int i = 7;i >= 0;i--) printf("%08X ", Y3[i]); printf("\n");
    printf("Z3: "); for (int i = 7;i >= 0;i--) printf("%08X ", Z3[i]); printf("\n");
    Ja2Af(X3, Y3, Z3, x3, y3, &is_inf3);
    printf("x3: "); for (int i = 7;i >= 0;i--) printf("%08X ", x3[i]); printf("\n");
    printf("y3: "); for (int i = 7;i >= 0;i--) printf("%08X ", y3[i]); printf("\n");

    /*int v = 3;
    unsigned int* k_n = (unsigned int*)calloc(v, 32);
    int* w_n = (int*)calloc(v, 4);
    unsigned int* x_n = (unsigned int*)calloc(v, 32);
    unsigned int* y_n = (unsigned int*)calloc(v, 32);
    int* is_inf_n = (int*)calloc(v, 4);

    k_n[0] = 2; k_n[8] = 2; k_n[16] = 1;
    w_n[0] = 2; w_n[1] = 3; w_n[2] = 4;
    memcpy(x_n, x1, 32); memcpy(y_n, y1, 32);
    memcpy(x_n + 8, x1, 32); memcpy(y_n + 8, y1, 32);
    memcpy(x_n + 16, x1, 32); memcpy(y_n + 16, y1, 32);
    ECCMSMInterleave(v, k_n, x_n, y_n, is_inf_n, X3, Y3, Z3, w_n);

    printf("X3: "); for (int i = 7;i >= 0;i--) printf("%08X ", X3[i]); printf("\n");
    printf("Y3: "); for (int i = 7;i >= 0;i--) printf("%08X ", Y3[i]); printf("\n");
    printf("Z3: "); for (int i = 7;i >= 0;i--) printf("%08X ", Z3[i]); printf("\n");
    Ja2Af(X3, Y3, Z3, x3, y3, &is_inf3);
    printf("x3: "); for (int i = 7;i >= 0;i--) printf("%08X ", x3[i]); printf("\n");
    printf("y3: "); for (int i = 7;i >= 0;i--) printf("%08X ", y3[i]); printf("\n");
    free(k_n); free(w_n); free(x_n); free(y_n); free(is_inf_n);*/
}

int main() {
    //ECADDAfAfTest();
    //ECDBLAfTest();
    //Ja2AfTest();
    //ECADDJaAfTest();
    //ECADDJaJaTest();
    //ECDBLJaTest();
    ECCSMTest();

    return 0;
}