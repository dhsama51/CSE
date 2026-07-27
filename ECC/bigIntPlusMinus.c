#include "bigIntPlusMinus.h"
#include "bigIntGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    if (*carry == 1 || bigIntCmp(temp, Fp[0], len) >= 0) { bigIntMinus(temp, Fp[0], carry, c, len); }
    else memcpy(c, temp, len * 4);
}

void FpMinus(unsigned int a[], unsigned int b[], int* borrow, unsigned int c[], int len) {
    unsigned int A[8] = { 0, }; unsigned int B[8] = { 0, };

    *borrow = 0;
    unsigned int temp[16] = { 0, };
    for (int i = 0;i < 8;i++) {
        temp[i] = a[i] - b[i] - *borrow;
        if ((a[i] < b[i]) || (a[i] == b[i] && *borrow == 1)) *borrow = 1;
        else *borrow = 0;
    }
    int carry = 0;
    if (*borrow == 1) bigIntPlus(temp, Fp[0], &carry, c, len);
    else if (bigIntCmp(temp, Fp[0], len) >= 0) bigIntMinus(temp, Fp[0], borrow, c, len);
    else memcpy(c, temp, len * 4);
}
