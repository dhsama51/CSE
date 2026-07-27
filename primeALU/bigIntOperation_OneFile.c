#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bit(a, n) (((a) & (1 << (n))) >> (n))
unsigned int Fp[5][8] = {
    { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF },
    { 0xFFFFFFFE, 0x00000002, 0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE },
    { 0xFFFFFFFD, 0x00000003, 0x00000000, 0x00000000, 0x00000002, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFD },
    { 0xFFFFFFFC, 0x00000004, 0x00000000, 0x00000000, 0x00000003, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFC },
    { 0xFFFFFFFB, 0x00000005, 0x00000000, 0x00000000, 0x00000004, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFB }
};


void endianChange(unsigned int input[], int len) {
    for (int i = 0;i < len / 2;i++) {
        unsigned int temp = input[i];
        input[i] = input[len - 1 - i];
        input[len - 1 - i] = temp;
    }
}

int bigIntInput(unsigned int data[], FILE* fp, int len) {
    for (int i = len - 1;i >= 0;i--) {
        if (fscanf(fp, " %08X", data + i) == EOF) return(-1);
    }
    for (int i = len - 1;i >= 0;i--) printf("%08X", data[i]); printf("\n");
}

void bigIntOutput(unsigned int data1[], int data2, FILE* fp, int len) {
    if (!fp) { printf("[bigIntOutput] fp is NULL\n"); return; }
    for (int i = len - 1;i >= 0;i--) {
        fprintf(fp, "%08X", data1[i]);
    }
    fprintf(fp, " %X\n\n", data2);
    for (int i = len - 1;i >= 0;i--) printf("%08X", data1[i]); printf(" %X\n\n", data2);
}

void bigIntPlus(unsigned int a[], unsigned int b[], int* carry, unsigned int c[], int len) {
    *carry = 0;
    for (int i = 0;i < len;i++) {
        /*방법 1. bit 단위 비트 연산. 단점: 느림
        for (int j = 0;j < 8 * 4;j++) {
            c[i] |= (bit(a[i], j) ^ bit(b[i], j) ^ *carry) << j;
            *carry = ((bit(a[i], j) & bit(b[i], j)) | (bit(a[i], j) & *carry) | (bit(b[i], j) & *carry));
        }*/

        /*방법 2. 64bit로 형변환하여 덧셈. 단점: 억지로 64bit로 늘려서 우회함, 1228bit 자료형이 없음
        long long temp = (long long)a[i] + b[i] + *carry;
        c[i] = (int)temp;
        *carry = temp >> 32;*/

        //방법 3. 주어진 bit 내에서 구현, 자료의 내용과 일치. 단점: 방법 2보다 코드가 더 긺
        unsigned int A = a[i];
        unsigned int B = b[i];
        c[i] = A + B;
        int carryTest = (c[i] < A);
        c[i] += *carry;
        *carry = carryTest | (c[i] < *carry);
    }
    //for (int i = 0;i < 8;i++) printf("%08X ", c[i]); printf("%d\n", *carry);
}

void bigIntPlusTest() {
    unsigned int data1[8] = { 0, }; unsigned int data2[8] = { 0, };
    unsigned int result[8] = { 0, }; int carry = 0;

    memset(data1, 0xff, 32);
    memset(data2, 0xff, 32);
    //memset(data1 + 5, 0xff, 12); data1[6]--;
    //memset(data2 + 4, 0xff, 16); memset(data2 + 6, 0, 4);
    for (int i = 0;i < 8;i++) printf("%08X ", data1[i]); printf("\n");
    for (int i = 0;i < 8;i++) printf("%08X ", data2[i]); printf("\n");
    bigIntPlus(data1, data2, &carry, result, 8);
}

void bigIntMinus(unsigned int a[], unsigned int b[], int* borrow, unsigned int c[], int len) {
    *borrow = 0;
    for (int i = 0;i < len;i++) {
        unsigned int A = a[i];
        unsigned int B = b[i];
        c[i] = A - B - *borrow;
        if ((A < B) || (A == B && *borrow == 1)) *borrow = 1;
        else *borrow = 0;
    }
    //for (int i = 0;i < 8;i++) printf("%08X ", c[i]); printf("%d\n", *borrow);
}

void bigIntMinusTest() {
    unsigned int data1[8] = { 0, }; unsigned int data2[8] = { 0, };
    unsigned int result[8] = { 0, }; int borrow = 0;

    memset(data1 + 5, 0xff, 4); //data1[6]--;
    memset(data2 + 4, 0xff, 16); memset(data2 + 5, 0, 8);
    for (int i = 7;i >= 0;i--) printf("%08X ", data1[i]); printf("\n");
    for (int i = 7;i >= 0;i--) printf("%08X ", data2[i]); printf("\n");
    endianChange(data1, 8);
    endianChange(data2, 8);
    bigIntMinus(data1, data2, &borrow, result, 8);
}

void bigIntPlusMinusTest() {
    FILE* fp1;
    fp1 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\add&sub\\TV_opA.txt", "r");
    FILE* fp2;
    fp2 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\add&sub\\TV_opB.txt", "r");
    FILE* fp3;
    fp3 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\add&sub\\TV_ADD_LDH.txt", "w");
    //fp3 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\add&sub\\TV_SUB_LDH.txt", "w");

    unsigned int data1[8] = { 0, }; unsigned int data2[8] = { 0, };
    unsigned int result[8] = { 0, }; int carry; int borrow;

    while (1) {
        bigIntInput(data1, fp1, 8); // 1줄만 복사
        bigIntInput(data2, fp2, 8); // 1줄만 복사
        bigIntPlus(data1, data2, &carry, result, 8);
        //bigIntMinus(data1, data2, &borrow, result, 8);
        bigIntOutput(result, carry, fp3, 8); // 1줄만 복사
        //bigIntOutput(result, borrow, fp3, 8); // 1줄만 복사
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
}

int bigIntCmp(unsigned int data1[], unsigned int data2[], int len) {
    for (int i = len - 1; i >= 0; i--) {
        if (data1[i] > data2[i]) return 1;
        if (data1[i] < data2[i]) return -1;
    }
    return 0;
}

void FpPlus(unsigned int a[], unsigned int b[], int* carry, unsigned int c[], int len) {
    *carry = 0;
    unsigned int temp[16] = { 0, };
    for (int i = 0;i < 8;i++) {
        temp[i] = a[i] + b[i];
        int carryTest = (temp[i] < a[i]);
        temp[i] += *carry;
        *carry = carryTest | (temp[i] < *carry);
    }
    int borrow = 0;
    if (*carry == 1 || bigIntCmp(temp, Fp[0], len) > 0) { bigIntMinus(temp, Fp[0], &borrow, c, len); }
    else memcpy(c, temp, 8 * 4);
    //for (int i = 0;i < 8;i++) printf("%08X", temp[i]);printf("\n");
}

void FpPlusTest() {
    unsigned int data1[8] = { 0, }; unsigned int data2[8] = { 0, };
    unsigned int result[8] = { 0, }; int carry = 0;

    memset(data1, 0xff, 32);
    memset(data2, 0xff, 32);
    //memset(data1 + 5, 0xff, 12); data1[6]--;
    //memset(data2 + 4, 0xff, 16); memset(data2 + 6, 0, 4);
    for (int i = 0;i < 8;i++) printf("%08X ", data1[i]); printf("\n");
    for (int i = 0;i < 8;i++) printf("%08X ", data2[i]); printf("\n");
    FpPlus(data1, data2, &carry, result, 8);
}

void FpMinus(unsigned int a[], unsigned int b[], int* borrow, unsigned int c[], int len) {
    *borrow = 0;
    unsigned int temp[16] = { 0, };
    for (int i = 0;i < 8;i++) {
        temp[i] = a[i] - b[i] - *borrow;
        if ((a[i] < b[i]) || (a[i] == b[i] && *borrow == 1)) *borrow = 1;
        else *borrow = 0;
    }
    int carry = 0;
    if (*borrow == 1) bigIntPlus(temp, Fp[0], &carry, c, len);
    else if (bigIntCmp(temp, Fp[0], len) == 1) bigIntMinus(temp, Fp[0], borrow, c, len);
    else memcpy(c, temp, 32);
    //for (int i = 0;i < 8;i++) printf("%08X", c[i]); printf("\n");
}

void FpMinusTest() {
    unsigned int data1[8] = { 0, }; unsigned int data2[8] = { 0, };
    unsigned int result[8] = { 0, }; int borrow = 0;

    memset(data1, 0xff, 32); //data1[6]--;
    memset(data2, 0xff, 32); //memset(data2 + 5, 0, 8);
    for (int i = 0;i < 8;i++) printf("%08X ", data1[i]); printf("\n");
    for (int i = 0;i < 8;i++) printf("%08X ", data2[i]); printf("\n");
    FpMinus(data1, data2, &borrow, result, 8);
}

void FpPlusMinusTest() {
    FILE* fp1;
    fp1 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\add&sub\\TV_opA.txt", "r");
    FILE* fp2;
    fp2 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\add&sub\\TV_opB.txt", "r");
    FILE* fp3;
    fp3 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\add&sub\\TV_FPADD_LDH.txt", "w");
    //fp3 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\add&sub\\TV_FPSUB_LDH.txt", "w");

    unsigned int data1[8] = { 0, }; unsigned int data2[8] = { 0, };
    unsigned int result[8] = { 0, }; int carry; int borrow;

    while (1) {
        bigIntInput(data1, fp1, 8); // 1줄만 복사
        bigIntInput(data2, fp2, 8); // 1줄만 복사
        FpPlus(data1, data2, &carry, result, 8);
        //FpMinus(data1, data2, &borrow, result, 8);
        bigIntOutput(result, carry, fp3, 8); // 1줄만 복사
        //bigIntOutput(result, borrow, fp3, 8); // 1줄만 복사
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
}

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
    //for (int i = 15;i >= 0;i--) printf("%08X ", c[i]); printf("\n");
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
    //for (int i = 15;i >= 0;i--) printf("%08X ", c[i]); printf("\n");
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

    bigIntPlus(c + size, H, &dummy, c + size, 2 * size);

    if (size == 8) {
        for (int i = 15;i >= 0;i--) printf("%08X ", c[i]);
        printf("\n");
    }
    return;
}

void bigIntMulTest() {
    unsigned int data1[8] = { 0, }; unsigned int data2[8] = { 0, };
    unsigned int result[16] = { 0, };

    char str1[] = "7E781AF075DF34221947833F570DC5EEC844DE0DE81A7071CDB374444857A994";
    char str2[] = "9C3980B6BFA0B75993BE648924B9D298101E46FEF0AC62E19D6BCABB8FF8D324";
    for (int i = 7;i >= 0;i--) sscanf(str1 + 8 * (7 - i), " %08X", data1 + i);
    for (int i = 7;i >= 0;i--) sscanf(str2 + 8 * (7 - i), " %08X", data2 + i);

    for (int i = 7;i >= 0;i--) printf("%08X ", data1[i]); printf("\n");
    for (int i = 7;i >= 0;i--) printf("%08X ", data2[i]); printf("\n");
    //bigIntMulOS(data1, data2, result);
    //bigIntMulPS(data1, data2, result);
    bigIntMulKaratsuba(data1, data2, 8, result);
}

void bigIntSquare(unsigned int a[], unsigned int c[], int len) {
    unsigned int R0 = 0, R1 = 0, R2 = 0;
    for (int k = 0;k <= 14;k++) {
        for (int i = (k < 7) ? k : 7, j = k - i;j <= i;) {
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
    c[15] = R0;

    //for (int i = 15;i >= 0;i--) printf("%08X ", c[i]); printf("\n");
}

void bigIntSquareTest() {
    unsigned int data[8] = { 0, };
    unsigned int result[16] = { 0, };

    char str[] = "7E781AF075DF34221947833F570DC5EEC844DE0DE81A7071CDB374444857A994";
    for (int i = 7;i >= 0;i--) sscanf(str + 8 * (7 - i), " %08X", data + i);

    for (int i = 7;i >= 0;i--) printf("%08X ", data[i]); printf("\n");
    bigIntSquare(data, result, 8);
}

void bigIntMulSquareTest() {
    FILE* fp1;
    fp1 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\mul&squ\\TV_opA.txt", "r");
    FILE* fp2;
    fp2 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\mul&squ\\TV_opB.txt", "r");
    FILE* fp3;
    //fp3 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\mul&squ\\TV_MUL_LDH.txt", "w");
    //fp3 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\mul&squ\\TV_MUL_LDH2.txt", "w");
    //fp3 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\mul&squ\\TV_MUL_LDH3.txt", "w");
    fp3 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\mul&squ\\TV_SQR_LDH.txt", "w");

    unsigned int data1[8] = { 0, }; unsigned int data2[8] = { 0, };
    unsigned int result[16] = { 0, }; int carry; int borrow;

    while (1) {
        bigIntInput(data1, fp1, 8); // 1줄만 복사
        //bigIntInput(data2, fp2, 8); // 1줄만 복사
        //bigIntMulOS(data1, data2, result);
        //bigIntMulPS(data1, data2, result);
        //bigIntMulKaratsuba(data1, data2, 8, result);
        bigIntSquare(data1, result, 8);
        bigIntOutput(result, borrow, fp3, 16); // 1줄만 복사
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
}

void bigIntReduct(unsigned int a[], unsigned c[], int len) {
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
        bigIntMinus(c, Fp[carry_sum - 1], &carry, c, 8);
    else if (carry_sum < 0)
        bigIntPlus(c, Fp[-1 - carry_sum], &borrow, c, 8);

    //for (int i = 7;i >= 0;i--) printf("%08X", c[i]); printf("\n");
}

void bigIntReductTest() {
    FILE* fp1;
    fp1 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\reduction\\TV_MUL_TV.txt", "r");
    FILE* fp2;
    fp2 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\reduction\\TV_RED_LDH.txt", "w");

    unsigned int data[16] = { 0, };
    unsigned int result[8] = { 0, }; int dummy = 0;

    while (1) {
        bigIntInput(data, fp1, 16); // 1줄만 복사
        bigIntReduct(data, result, 8);
        bigIntOutput(result, dummy, fp2, 8); // 1줄만 복사
    }


    /*char str[] = "E0FA64F40B0204C9A245A2E1BA35254BF342087CC4600BE85AB506E36FA5F190AB84085811050B0F4372273FA5522A106EE1785D607CDA60DF85CBC246CD3D1F";
    for (int i = 15;i >= 0;i--) sscanf(str + 8 * (15 - i), " %08X", data + i);
    for (int i = 15;i >= 0;i--) printf("%08X ", data[i]); printf("\n");
    bigIntReduct(data, result, 16);*/


    fclose(fp1);
    fclose(fp2);
}

int main() {
    unsigned int data1[8] = { 0, }; unsigned int data2[8] = { 0, };
    unsigned int result[8] = { 0, }; int carry; int borrow;

    //bigIntInput(data1, fp1, 8); //1줄만 복사
    //bigIntPlusTest();
    //bigIntMinusTest();
    //bigIntOutput(result, carry, fp3, 8); //1줄만 복사
    //bigIntPlusMinusTest();

    //FpPlusTest();
    //FpMinusTest();
    //FpPlusMinusTest();

    //bigIntMulTest();
    //bigIntSquareTest();
    //bigIntMulSquareTest();

    bigIntReductTest();

    return 0;
}