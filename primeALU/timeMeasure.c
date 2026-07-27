#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <intrin.h>

void timeMeasure() {
    int sub[2] = { 0, };
    int startClock; int stopClock;
    int startTime; int stopTime;
    LARGE_INTEGER FrequencyQPC; LARGE_INTEGER startQPC; LARGE_INTEGER endQPC;
    unsigned long long startRDTSC; unsigned long long endRDTSC;

    QueryPerformanceFrequency(&FrequencyQPC);
    startClock = clock(); startTime = time(NULL);
    QueryPerformanceCounter(&startQPC);
    startRDTSC = __rdtsc();

    for (int i = 0;i < 1000000000;i++) {
        sub[(i + 1) % 2] = sub[i % 2];
    }

    stopClock = clock();
    stopTime = time(NULL);
    QueryPerformanceCounter(&endQPC);
    endRDTSC = __rdtsc();

    double durationClock = (double)(stopClock - startClock) / CLOCKS_PER_SEC;
    double durationTime = (double)difftime(stopTime, startTime);
    __int64 elapsedQPC = endQPC.QuadPart - startQPC.QuadPart;
    double durationQPC = (double)elapsedQPC / (double)FrequencyQPC.QuadPart;
    unsigned long long clockRDTSC = endRDTSC - startRDTSC;

    printf("%lf\n", durationClock); //클럭 차 / (clock/sec)
    printf("%lf\n", durationTime); //초 단위
    printf("%lf\n", durationQPC); //Freq, Counter 계산
    printf("clocks: %lld\n", clockRDTSC / elapsedQPC); //rdtsc 레지스터로 clock 계산
}

int main() {
    timeMeasure();

    return 0;
}