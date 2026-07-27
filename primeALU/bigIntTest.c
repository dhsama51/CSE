#include "bigIntIO.h"
#include "bigIntPlusMinus.h"
#include "bigIntMulSqrRed.h"
#include "bigIntTest.h"
#include "bigIntGlobal.h"
#include "bigIntInv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void endianChange(unsigned int input[], int len) {
    for (int i = 0;i < len / 2;i++) {
        unsigned int temp = input[i];
        input[i] = input[len - 1 - i];
        input[len - 1 - i] = temp;
    }
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
        if (bigIntInput(data1, fp1, 8) == -1) return; // 1줄만 복사
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
    //fp3 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\add&sub\\TV_FPADD_LDH.txt", "w");
    fp3 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\add&sub\\TV_FPSUB_LDH.txt", "w");

    unsigned int data1[8] = { 0, }; unsigned int data2[8] = { 0, };
    unsigned int result[8] = { 0, }; int carry; int borrow;

    while (1) {
        if (bigIntInput(data1, fp1, 8) == -1) return; // 1줄만 복사
        bigIntInput(data2, fp2, 8); // 1줄만 복사
        //FpPlus(data1, data2, &carry, result, 8);
        FpMinus(data1, data2, &borrow, result, 8);
        //bigIntOutput(result, carry, fp3, 8); // 1줄만 복사
        bigIntOutput(result, borrow, fp3, 8); // 1줄만 복사
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
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
        if (bigIntInput(data1, fp1, 8) == -1) return; // 1줄만 복사
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

void bigIntReductTest() {
    FILE* fp1;
    fp1 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\reduction\\TV_MUL_TV.txt", "r");
    FILE* fp2;
    fp2 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\reduction\\TV_RED_LDH.txt", "w");

    unsigned int data[16] = { 0, };
    unsigned int result[8] = { 0, }; int dummy = 0;

    while (1) {
        if (bigIntInput(data, fp1, 16) == -1) return; // 1줄만 복사
        FpReduct(data, result, 8);
        bigIntOutput(result, dummy, fp2, 8); // 1줄만 복사
    }


    /*char str[] = "E0FA64F40B0204C9A245A2E1BA35254BF342087CC4600BE85AB506E36FA5F190AB84085811050B0F4372273FA5522A106EE1785D607CDA60DF85CBC246CD3D1F";
    for (int i = 15;i >= 0;i--) sscanf(str + 8 * (15 - i), " %08X", data + i);
    for (int i = 15;i >= 0;i--) printf("%08X ", data[i]); printf("\n");
    FpReduct(data, result, 8);*/


    fclose(fp1);
    fclose(fp2);
}

void bigIntInvTest() {

    FILE* fp1;
    fp1 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\inv\\TV_opA.txt", "r");
    FILE* fp2;
    //fp2 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\inv\\TV_INV1_LDH.txt", "w");
    fp2 = fopen("C:\\Users\\user\\Desktop\\Study\\code\\CSE\\primeALU\\inv\\TV_INV2_LDH.txt", "w");

    unsigned int data[16] = { 0, };
    unsigned int result[8] = { 0, }; int dummy = 0;

    while (1) {
        if (bigIntInput(data, fp1, 8) == -1) return; // 1줄만 복사
        //bigIntInvBin(data, result, 8);
        bigIntInvFermat(data, result, 8);
        bigIntOutput(result, dummy, fp2, 8); // 1줄만 복사
    }

    /*char str[] = "EAC8C20CE80D90B3BF6DC08AFD4C8B1D6274839279D9F1469BE87CCD85283FE5";
    unsigned int data[8] = { 0, };
    unsigned int result[8] = { 0, };
    for (int i = 7;i >= 0;i--) sscanf(str + 8 * (7 - i), " %08X", data + i);

    bigIntInvBin(data, result, 8);*/

    fclose(fp1);
    fclose(fp2);
}