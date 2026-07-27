#include "bigIntIO.h"
#include "bigIntGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bigIntInput(unsigned int data[], FILE* fp, int len) {
    for (int i = len - 1;i >= 0;i--) {
        if (fscanf(fp, " %08X", data + i) == EOF) return -1;
    }
    //for (int i = len - 1;i >= 0;i--) printf("%08X", data[i]); printf("\n");
    return 0;
}

void bigIntOutput(unsigned int data1[], int data2, FILE* fp, int len) {
    if (!fp) { return; }
    for (int i = len - 1;i >= 0;i--) {
        fprintf(fp, "%08X", data1[i]);
    }
    fprintf(fp, " %X\n\n", data2);
    //for (int i = len - 1;i >= 0;i--) printf("%08X", data1[i]); printf(" %X\n\n", data2);
}
