#include <stdio.h>

int main() {
    unsigned int A[2] = { 0x01020304, 0x05060708 };
    unsigned char* B = (unsigned char*)A;
    for (int i = 0;i < 2;i++) printf("A[%d]\t\t\t\t    ", i);
    printf("\n");
    for (int i = 0;i < 8;i++) printf("B[%d]     ", i);
    printf("\n");
    for (int i = 0;i < 8;i++) printf("%02x       ", B[i]);
    printf("\n");
    for (int i = 0;i < 64;i++) {
        printf("%d", (B[i / 8] >> (7 - i % 8)) & 1);
        if (i % 8 == 7) printf(" ");
    }

    return 0;
}