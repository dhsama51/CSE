#include "bigIntIO.h"
#include "bigIntPlusMinus.h"
#include "bigIntMulSqrRed.h"
#include "bigIntTest.h"
#include "bigIntGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

int main() {
    unsigned int data1[8] = { 0, }; unsigned int data2[8] = { 0, };
    unsigned int result[8] = { 0, }; int carry; int borrow;

    LARGE_INTEGER FrequencyQPC, startQPC, endQPC;
    unsigned long long startRDTSC, endRDTSC;

    QueryPerformanceFrequency(&FrequencyQPC);
    QueryPerformanceCounter(&startQPC);
    startRDTSC = __rdtsc();

    //bigIntInput(data1, fp1, 8);
    //bigIntPlusTest();
    //bigIntMinusTest();
    //bigIntOutput(result, carry, fp3, 8);
    //bigIntPlusMinusTest();

    //FpPlusTest();
    //FpMinusTest();
    //FpPlusMinusTest();

    //bigIntMulTest();
    //bigIntSquareTest();
    //bigIntMulSquareTest();

    //bigIntReductTest();
    bigIntInvTest();

    endRDTSC = __rdtsc();
    QueryPerformanceCounter(&endQPC);

    __int64 elapsedQPC = endQPC.QuadPart - startQPC.QuadPart;
    double elapsedTime = (double)elapsedQPC / (double)FrequencyQPC.QuadPart;
    unsigned long long elapsedCycle = endRDTSC - startRDTSC;

    double sec_per_cycle = elapsedTime / (double)elapsedCycle;
    printf("Total cycles: %llu\n", elapsedCycle);
    printf("Total time  : %.9f sec\n", elapsedTime);
    printf("sec/cycle   : %.12e\n", sec_per_cycle);

    return 0;
}