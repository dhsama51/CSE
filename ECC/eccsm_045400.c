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

void ECCSMLtRJa(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int X3[], unsigned int Y3[], unsigned int Z3[]) {
    memset(X3, 0, 32); memset(Y3, 0, 32); memset(Z3, 0, 32);
    unsigned int tempX[8] = { 1, 0, }, tempY[8] = { 1, 0, }, tempZ[8] = { 0, };

    for (int j = 7;j >= 0;j--) {
        for (int i = 31;i >= 0;i--) {
            ECDBLJa(tempX, tempY, tempZ, tempX, tempY, tempZ);
            if (k[j] & (1 << i)) ECADDJaAf(tempX, tempY, tempZ, x1, y1, is_inf1, tempX, tempY, tempZ);
        }
    }
    memcpy(X3, tempX, 32); memcpy(Y3, tempY, 32); memcpy(Z3, tempZ, 32);
}

void ECCSMLtRAf(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int x3[], unsigned int y3[], int* is_inf3) {
    memset(x3, 0, 32); memset(y3, 0, 32); *is_inf3 = 1;
    unsigned int tempx[8] = { 0, }, tempy[8] = { 0, }; int tempis_inf3 = 1;

    for (int j = 7;j >= 0;j--) {
        for (int i = 31;i >= 0;i--) {
            ECDBLAf(tempx, tempy, tempis_inf3, tempx, tempy, &tempis_inf3);
            if (k[j] & (1 << i)) ECADDAfAf(tempx, tempy, tempis_inf3, x1, y1, is_inf1, tempx, tempy, &tempis_inf3);
        }
    }
    memcpy(x3, tempx, 32); memcpy(y3, tempy, 32); *is_inf3 = tempis_inf3;
}

void ECCSMRtLAf(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int x3[], unsigned int y3[], int* is_inf3) {
    memset(x3, 0, 32); memset(y3, 0, 32); *is_inf3 = 1;
    unsigned int tempx[8] = { 0, };
    unsigned int tempy[8] = { 0, };
    int tempis_inf3 = 1;

    unsigned int x1c[8] = { 0, }, y1c[8] = { 0, };
    memcpy(x1c, x1, 32); memcpy(y1c, y1, 32);
    int is_inf1c = is_inf1;

    for (int j = 7;j >= 0;j--) {
        for (int i = 0;i < 32;i++) {
            if (k[j] & (1 << i)) ECADDAfAf(tempx, tempy, tempis_inf3, x1c, y1c, is_inf1c, tempx, tempy, &tempis_inf3);
            ECDBLAf(x1c, y1c, is_inf1c, x1c, y1c, &is_inf1c);
        }
    }
    memcpy(x3, tempx, 32); memcpy(y3, tempy, 32); *is_inf3 = tempis_inf3;
}

int int2NAF(unsigned int k[], char kNAF[]) {
    unsigned int k_[8]; memcpy(k_, k, 32);
    int i = 0;
    unsigned int zero8[8] = { 0, };
    unsigned int one8[8] = { 1,0, };
    char temp = 0;
    int dummy = 0;

    while (bigIntCmp(k_, zero8, 8) != 0) {
        if (k_[0] & 1) {
            temp = 2 - (k_[0] % 4);
            if (temp == 1) bigIntMinus(k_, one8, &dummy, k_, 8);
            else if (temp == -1) bigIntPlus(k_, one8, &dummy, k_, 8);
        }

        else {
            temp = 0;
        }
        kNAF[i] = temp;

        rightShiftNbit(k_, 1, 8); i++;
    }
    return i;
}

void ECCSMLtRNAF(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int X3[], unsigned int Y3[], unsigned int Z3[]) {
    char kNAF[258] = "";
    int l = int2NAF(k, kNAF);

    memset(X3, 0, 32); memset(Y3, 0, 32); memset(Z3, 0, 32);
    unsigned int tempX[8] = { 1, 0, }, tempY[8] = { 1, 0, }, tempZ[8] = { 0, };

    for (int i = l - 1;i >= 0;i--) {
        ECDBLJa(tempX, tempY, tempZ, tempX, tempY, tempZ);
        if (kNAF[i] == 1) {
            ECADDJaAf(tempX, tempY, tempZ, x1, y1, is_inf1, tempX, tempY, tempZ);
        }
        else if (kNAF[i] == -1) {
            unsigned int y1_[8] = { 0, }; int dummy = 0;
            FpMinus(Fp[0], y1, &dummy, y1_, 8);
            ECADDJaAf(tempX, tempY, tempZ, x1, y1_, is_inf1, tempX, tempY, tempZ);
        }
    }

    memcpy(X3, tempX, 32); memcpy(Y3, tempY, 32); memcpy(Z3, tempZ, 32);
}

int int2wNAF(unsigned int k[], char kNAF[], int w) {
    unsigned int k_[8]; memcpy(k_, k, 32);
    int i = 0;
    unsigned int zero8[8] = { 0, };
    char temp = 0;
    int dummy = 0;

    int window = 1 << w;
    int half = 1 << (w - 1);

    while (bigIntCmp(k_, zero8, 8) != 0) {
        if (k_[0] & 1) {
            temp = (k_[0] % window);
            if (temp >= half) temp -= window;
            if (temp > 0) {
                unsigned int t[8] = { 0, }; t[0] = (unsigned int)temp;
                bigIntMinus(k_, t, &dummy, k_, 8);
            }
            else if (temp < 0) {
                unsigned int t[8] = { 0, }; t[0] = (unsigned int)(-temp);
                bigIntPlus(k_, t, &dummy, k_, 8);
            }
        }
        else {
            temp = 0;
        }
        kNAF[i] = temp;

        rightShiftNbit(k_, 1, 8); i++;
    }
    return i;
}

void ECCSMLtRwNAF(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int X3[], unsigned int Y3[], unsigned int Z3[], int w) {
    char kNAF[258] = "";
    int l = int2wNAF(k, kNAF, w);
    int window = 1 << w;
    int half = 1 << (w - 1);

    unsigned int nx1[8 * half], ny1[8 * half];
    memcpy(nx1, x1, 32); memcpy(ny1, y1, 32);

    unsigned int tempX[8] = { 0, }, tempY[8] = { 0, }, tempZ[8] = { 0, };
    for (int i = 1;i < half / 2;i++) {
        unsigned int n[8] = { 0, };
        n[0] += i * 2 + 1;
        ECCSMLtRJa(n, x1, y1, is_inf1, tempX, tempY, tempZ);
        Ja2Af(tempX, tempY, tempZ, nx1 + i * 8, ny1 + i * 8, &is_inf1);
    }

    memset(X3, 0, 32); memset(Y3, 0, 32); memset(Z3, 0, 32);
    memset(tempX, 0, 32); memset(tempY, 0, 32); memset(tempZ, 0, 32);
    tempX[0]++; tempY[0]++;

    for (int i = l - 1;i >= 0;i--) {
        ECDBLJa(tempX, tempY, tempZ, tempX, tempY, tempZ);
        if (kNAF[i] != 0) {
            int abs_kNAF = (kNAF[i] > 0) ? kNAF[i] : -kNAF[i];
            int offset = (abs_kNAF / 2) * 8;

            if (kNAF[i] > 0) {
                ECADDJaAf(tempX, tempY, tempZ, nx1 + offset, ny1 + offset, is_inf1, tempX, tempY, tempZ);
            }
            else if (kNAF[i] < 0) {
                int dummy = 0;
                unsigned int ny1_[8] = { 0, };
                FpMinus(Fp[0], ny1 + offset, &dummy, ny1_, 8);
                ECADDJaAf(tempX, tempY, tempZ, nx1 + offset, ny1_, is_inf1, tempX, tempY, tempZ);
            }
        }
    }

    memcpy(X3, tempX, 32); memcpy(Y3, tempY, 32); memcpy(Z3, tempZ, 32);
}

void ECCSMSlideWin(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int X3[], unsigned int Y3[], unsigned int Z3[], int w) {
    char kNAF[258] = "";
    int l = int2NAF(k, kNAF);
    int value = ((1 << w) - (w % 2 == 0 ? 1 : -1)) / 3;

    unsigned int tempX[8] = { 0, }; unsigned int tempY[8] = { 0, }; unsigned int tempZ[8] = { 0, };
    unsigned int nx1[8 * value], ny1[8 * value];
    memset(nx1, 0, 32 * value); memset(ny1, 0, 32 * value);
    memcpy(nx1, x1, 32); memcpy(ny1, y1, 32);
    for (int i = 1; i < value;i++) {
        unsigned int n[8] = { 0, };
        n[0] += i * 2 + 1;
        ECCSMLtRJa(n, x1, y1, is_inf1, tempX, tempY, tempZ);
        Ja2Af(tempX, tempY, tempZ, nx1 + 8 * i, ny1 + 8 * i, &is_inf1);
    }

    memset(tempX, 0, 32); memset(tempY, 0, 32); memset(tempZ, 0, 32);
    tempX[0] += 1; tempY[0] += 1;
    int i = l - 1;
    while (i >= 0) {
        int t = 1, u = 0;
        if ((kNAF[i])) {
            int j = 1;
            while (j <= w && i - j + 1 >= 0) {
                if (kNAF[i - j + 1]) t = j;
                j++;
            }

            for (j = 0;j < t;j++) u += kNAF[i - j] * (1 << (t - j - 1));
        }

        for (int j = 0;j < t;j++) ECDBLJa(tempX, tempY, tempZ, tempX, tempY, tempZ);

        if (u) {
            int abs_u = (u > 0) ? u : -u;
            int offset = ((abs_u - 1) / 2) * 8;

            if (u > 0) ECADDJaAf(tempX, tempY, tempZ, nx1 + offset, ny1 + offset, is_inf1, tempX, tempY, tempZ);
            else if (u < 0) {
                unsigned int ny1_[8] = { 0, }; int dummy = 0;
                FpMinus(Fp[0], ny1 + offset, &dummy, ny1_, 8);
                ECADDJaAf(tempX, tempY, tempZ, nx1 + offset, ny1_, is_inf1, tempX, tempY, tempZ);
            }
        }
        i -= t;
    }
    memcpy(X3, tempX, 32); memcpy(Y3, tempY, 32); memcpy(Z3, tempZ, 32);
}

void ECCSMFixedWin(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int X3[], unsigned int Y3[], unsigned int Z3[], int w) {
    int d = (256 / w) + (256 % w != 0);
    unsigned int k_d[d]; memset(k_d, 0, 4 * d);
    for (int i = 0;i < d;i++) {
        int word_offset = w * i / 32;
        int bit_offset = w * i % 32;

        if (bit_offset + w <= 32) {
            k_d[i] = (k[word_offset] >> bit_offset) & ((1 << w) - 1);
        }
        else if (bit_offset + w > 32) {
            k_d[i] = (k[word_offset] >> bit_offset);
            if (word_offset + 1 < 8) {
                k_d[i] |= (k[word_offset + 1] << (32 - bit_offset));
            }
            k_d[i] &= ((1 << w) - 1);
        }
    }

    unsigned int x1_[8] = { 0, }; unsigned int y1_[8] = { 0, };
    unsigned int tempX[8] = { 0, }; unsigned int tempY[8] = { 0, }; unsigned int tempZ[8] = { 0, };
    unsigned int nx1[8 * d], ny1[8 * d];
    memset(nx1, 0, 32 * d); memset(ny1, 0, 32 * d);

    memcpy(nx1, x1, 32); memcpy(ny1, y1, 32);
    memcpy(tempX, x1, 32); memcpy(tempY, y1, 32); tempZ[0] += 1;
    for (int i = 1;i < d;i++) {
        for (int j = 0; j < w;j++) ECDBLJa(tempX, tempY, tempZ, tempX, tempY, tempZ);
        Ja2Af(tempX, tempY, tempZ, nx1 + 8 * i, ny1 + 8 * i, &is_inf1);
    }

    memset(tempX, 0, 32); memset(tempY, 0, 32); memset(tempZ, 0, 32);
    tempX[0] += 1; tempY[0] += 1;
    unsigned int temp2X[8] = { 0, }; unsigned int temp2Y[8] = { 0, }; unsigned int temp2Z[8] = { 0, };
    temp2X[0] += 1; temp2Y[0] += 1;
    unsigned int temp2x[8] = { 0, }, temp2y[8] = { 0, }, is_inf2 = 0;

    for (int j = (1 << w) - 1;j >= 1;j--) {
        for (int i = 0;i < d;i++) {
            if (k_d[i] == j) ECADDJaAf(temp2X, temp2Y, temp2Z, nx1 + i * 8, ny1 + i * 8, is_inf1, temp2X, temp2Y, temp2Z);
        }
        ECADDJaJa(tempX, tempY, tempZ, temp2X, temp2Y, temp2Z, tempX, tempY, tempZ);
    }

    memcpy(X3, tempX, 32); memcpy(Y3, tempY, 32); memcpy(Z3, tempZ, 32);
}

void ECCSMFixedComb(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int X3[], unsigned int Y3[], unsigned int Z3[], int w) {
    int d = (256 / w) + (256 % w != 0);

    unsigned int pre_X[8 * (1 << w)], pre_Y[8 * (1 << w)], pre_Z[8 * (1 << w)];
    memset(pre_X, 0, 32 * (1 << w)); memset(pre_Y, 0, 32 * (1 << w)); memset(pre_Z, 0, 32 * (1 << w));

    memcpy(pre_X, x1, 32); memcpy(pre_Y, y1, 32); pre_Z[0] = 1;
    memcpy(pre_X + 8, x1, 32); memcpy(pre_Y + 8, y1, 32); pre_Z[8] = 1;
    for (int i = 1;i < w;i++) {
        int idx = i * 8;
        for (int j = 0;j < d;j++)
            ECDBLJa(pre_X + idx, pre_Y + idx, pre_Z + idx, pre_X + idx, pre_Y + idx, pre_Z + idx);
        if (i != (d - 1)) {
            memcpy(pre_X + idx + 8, pre_X + idx, 32); memcpy(pre_Y + idx + 8, pre_Y + idx, 32); memcpy(pre_Z + idx + 8, pre_Z + idx, 32);
        }
    }

    unsigned int pre_x[8] = { 0, }, pre_y[8] = { 0, }; int pre_z = 0;
    unsigned int comb_X[8 * (1 << w)], comb_Y[8 * (1 << w)], comb_Z[8 * (1 << w)];
    memset(comb_X, 0, 32 * (1 << w)); memset(comb_Y, 0, 32 * (1 << w)); memset(comb_Z, 0, 32 * (1 << w));

    unsigned int n[8] = { 0, }; n[0] += 1;
    for (int i = 0;i < (1 << w);i++) {
        for (int j = 0;j < w;j++) {
            int iidx = i * 8, jidx = j * 8;
            if (i & (1 << j)) {
                ECADDJaJa(comb_X + iidx, comb_Y + iidx, comb_Z + iidx, pre_X + jidx, pre_Y + jidx, pre_Z + jidx, comb_X + iidx, comb_Y + iidx, comb_Z + iidx);
            }
        }
    }

    unsigned int tempX[8] = { 0, }, tempY[8] = { 0, }, tempZ[8] = { 0, };
    tempX[0] = 1, tempY[0] = 1;
    for (int i = d - 1;i >= 0;i--) {
        ECDBLJa(tempX, tempY, tempZ, tempX, tempY, tempZ);
        int idx = 0;
        for (int j = 0;j < w;j++) {
            if (j * d + i >= 256) continue;
            int word_offset = (j * d + i) / 32;
            int bit_offset = (j * d + i) % 32;
            if ((k[word_offset] >> bit_offset) & 1)
                idx += (1 << j);
        }
        ECADDJaJa(tempX, tempY, tempZ, comb_X + 8 * idx, comb_Y + 8 * idx, comb_Z + 8 * idx, tempX, tempY, tempZ);
    }
    memcpy(X3, tempX, 32); memcpy(Y3, tempY, 32); memcpy(Z3, tempZ, 32);
}

void ECCMSMShamir(unsigned int k[], unsigned int x1[], unsigned int y1[], unsigned int is_inf1, unsigned int l[], unsigned int x2[], unsigned int y2[], int is_inf2, unsigned int X3[], unsigned int Y3[], unsigned int Z3[], int w) {
    int d = (256 / w) + (256 % w != 0);

    unsigned int pre_X[8 * (1 << (w * 2))], pre_Y[8 * (1 << (w * 2))], pre_Z[8 * (1 << (w * 2))];
    memset(pre_X, 0, 32 * (1 << (w * 2))); memset(pre_Y, 0, 32 * (1 << (w * 2))); memset(pre_Z, 0, 32 * (1 << (w * 2)));

    pre_X[0] = 1; pre_Y[0] = 1;
    unsigned int pre_x[8] = { 0, }, pre_y[8] = { 0, };
    int pre_z = 0;

    memcpy(pre_X + 8, x1, 32); memcpy(pre_Y + 8, y1, 32); pre_Z[8] += 1;
    for (int i = 2;i < (1 << w);i++) {
        int cur = i * 8;
        int prev = i * 8 - 8;
        ECADDJaJa(pre_X + prev, pre_Y + prev, pre_Z + prev, pre_X + 8, pre_Y + 8, pre_Z + 8, pre_X + cur, pre_Y + cur, pre_Z + cur);
    }

    memcpy(pre_X + (1 << w) * 8, x2, 32); memcpy(pre_Y + (1 << w) * 8, y2, 32); *(pre_Z + (1 << w) * 8) += 1;
    for (int i = 2;i < (1 << w);i++) {
        int cur = (1 << w) * 8 * i;
        int prev = (1 << w) * 8 * (i - 1);
        ECADDJaJa(pre_X + prev, pre_Y + prev, pre_Z + prev, pre_X + (1 << w) * 8, pre_Y + (1 << w) * 8, pre_Z + (1 << w) * 8, pre_X + cur, pre_Y + cur, pre_Z + cur);
    }

    for (int i = 1;i < (1 << w);i++) {
        int Pcur = 8 * i;
        for (int j = 1;j < (1 << w);j++) {
            int Qcur = (1 << w) * 8 * j;
            int cur = Pcur + Qcur;
            ECADDJaJa(pre_X + Pcur, pre_Y + Pcur, pre_Z + Pcur, pre_X + Qcur, pre_Y + Qcur, pre_Z + Qcur, pre_X + cur, pre_Y + cur, pre_Z + cur);
        }
    }

    unsigned int tempX[8] = { 1,0, }, tempY[8] = { 1,0, }, tempZ[8] = { 0, };

    for (int i = d - 1;i >= 0;i--) {
        for (int j = 0;j < w;j++) ECDBLJa(tempX, tempY, tempZ, tempX, tempY, tempZ);
        int word_offset = i * w / 32;
        int bit_offset = i * w % 32;
        int k_i = (k[word_offset] >> bit_offset) & ((1 << w) - 1);
        int l_i = (l[word_offset] >> bit_offset) & ((1 << w) - 1);
        int cur = 8 * k_i + (1 << w) * 8 * l_i;
        ECADDJaJa(tempX, tempY, tempZ, pre_X + cur, pre_Y + cur, pre_Z + cur, tempX, tempY, tempZ);
    }

    memcpy(X3, tempX, 32); memcpy(Y3, tempY, 32); memcpy(Z3, tempZ, 32);
}

void ECCMSMInterleave(int v, unsigned int k_n[], unsigned int x_n[], unsigned int y_n[], int is_inf_n[], unsigned int X3[], unsigned int Y3[], unsigned int Z3[], int* w_n) {
    char kNAF_n[258 * v]; memset(kNAF_n, 0, 258 * v);

    int l_n[v]; memset(l_n, 0, 4 * v);
    int window_n[v]; memset(window_n, 0, 4 * v);
    int half_n[v]; memset(half_n, 0, 4 * v);

    for (int i = 0;i < v;i++) {
        l_n[i] = int2wNAF(k_n + 8 * i, kNAF_n + 258 * i, w_n[i]);
        window_n[i] = 1 << w_n[i];
        half_n[i] = 1 << (w_n[i] - 1);
    }

    int max_l = 0;
    for (int i = 0;i < v;i++)
        if (max_l < l_n[i]) max_l = l_n[i];

    int half_n_sum = 0;
    for (int i = 0;i < v;i++) half_n_sum += half_n[i];
    unsigned int nx_n[8 * half_n_sum], ny_n[8 * half_n_sum];
    memset(nx_n, 0, 32 * half_n_sum); memset(ny_n, 0, 32 * half_n_sum);

    int base[v]; memset(base, 0, 4 * v);
    unsigned int tempX[8] = { 0, }, tempY[8] = { 0, }, tempZ[8] = { 0, };
    for (int i = 0;i < v;i++) {
        memcpy(nx_n + base[i], x_n + 8 * i, 32); memcpy(ny_n + base[i], y_n + 8 * i, 32);

        for (int j = 1;j < half_n[i] / 2;j++) {
            unsigned int n[8] = { 0, };
            n[0] += j * 2 + 1;
            ECCSMLtRJa(n, x_n + 8 * i, y_n + 8 * i, is_inf_n[i], tempX, tempY, tempZ);
            Ja2Af(tempX, tempY, tempZ, nx_n + base[i] + j * 8, ny_n + base[i] + j * 8, is_inf_n + i);
        }
        if (i != (v - 1))
            base[i + 1] += base[i] + half_n[i] * 8;
    }

    memset(tempX, 0, 32); memset(tempY, 0, 32); memset(tempZ, 0, 32);
    tempX[0]++; tempY[0]++;

    for (int i = max_l - 1;i >= 0;i--) {
        ECDBLJa(tempX, tempY, tempZ, tempX, tempY, tempZ);
        for (int j = 0;j < v;j++) {
            if (kNAF_n[258 * j + i] != 0) {
                int abs_kNAF = (kNAF_n[258 * j + i] > 0) ? kNAF_n[258 * j + i] : -kNAF_n[258 * j + i];
                int offset = (abs_kNAF / 2) * 8;

                if (kNAF_n[258 * j + i] > 0) {
                    ECADDJaAf(tempX, tempY, tempZ, nx_n + base[i] + offset, ny_n + base[i] + offset, is_inf_n[i], tempX, tempY, tempZ);
                }
                else if (kNAF_n[258 * j + i] < 0) {
                    int dummy = 0;
                    unsigned int ny1_[8] = { 0, };
                    FpMinus(Fp[0], ny_n + base[i] + offset, &dummy, ny1_, 8);
                    ECADDJaAf(tempX, tempY, tempZ, nx_n + base[i] + offset, ny1_, is_inf_n[i], tempX, tempY, tempZ);
                }
            }
        }
    }

    memcpy(X3, tempX, 32); memcpy(Y3, tempY, 32); memcpy(Z3, tempZ, 32);
}
