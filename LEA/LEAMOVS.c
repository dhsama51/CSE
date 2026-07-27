#define _CRT_SECURE_NO_WARNINGS
#include "LEAGlobal.h"
#include "LEAEncDec.h"
#include "LEAPad.h"
#include "LEAMode.h"
#include "LEAMOVS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void LEA_ALL_ECB_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher) {
    char target[6][150] = { "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(ECB)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(ECB)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(ECB)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(ECB)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(ECB)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(ECB)MMT.txt" };
    int isSuccess[6] = { 0, };
    for (int z = 0;z < 6;z++) {
        FILE* fp;
        char input_str[16 * MAXBLOCK + 1] = { 0 };
        unsigned char compare[16 * MAXBLOCK] = { 0x00, };
        int count = 1;

        fp = fopen(target[z], "r");
        while (1) {
            if (fscanf(fp, " KEY = %s", input_str) == EOF) break;
            *keyLen = strlen(input_str) / 2;
            for (int i = 0;i < *keyLen;i++) sscanf(input_str + 2 * i, "%2hhx", key + i);
            keySchedule(*keyLen, key, encKey, decKey);

            fscanf(fp, " PT = %s", input_str);
            *plainLen = strlen(input_str) / 2;
            for (int i = 0;i < *plainLen;i++) sscanf(input_str + 2 * i, "%2hhx", plain + i);

            ECBenc(*plainLen, plain, *keyLen, encKey, cipherLen, cipher);

            fscanf(fp, " CT = %s", input_str);
            for (int i = 0;i < *cipherLen;i++) sscanf(input_str + 2 * i, "%2hhx", compare + i);

            printf("%dth test : ", count);
            for (int i = 0;i < *cipherLen;i++) printf("%02X", cipher[i]);
            for (int i = 0;i < *cipherLen;i++) {
                if (cipher[i] != compare[i]) {
                    printf(" Fail!");
                    return;
                }
            }
            printf(" Success\n");
            count += 1;
        }
        printf("%s complete\n\n", target[z]);
        isSuccess[z] = 1;
        fclose(fp);
    }
    printf("LEA-128-ECB KAT: %s, LEA-192-ECB KAT: %s, LEA-256-ECB KAT: %s", isSuccess[0] ? "Success" : "Fail", isSuccess[1] ? "Success" : "Fail", isSuccess[2] ? "Success\n" : "Fail\n");
    printf("LEA-128-ECB MMT: %s, LEA-192-ECB MMT: %s, LEA-256-ECB MMT: %s", isSuccess[3] ? "Success" : "Fail", isSuccess[4] ? "Success" : "Fail", isSuccess[5] ? "Success\n" : "Fail\n");
}

void LEA_ALL_CBC_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher) {
    char target[6][150] = { "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CBC)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CBC)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CBC)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CBC)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CBC)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CBC)MMT.txt" };
    int isSuccess[6] = { 0, };
    for (int z = 0;z < 6;z++) {
        FILE* fp;
        char input_str[16 * MAXBLOCK + 1] = { 0 };
        unsigned char compare[16 * MAXBLOCK] = { 0x00, };
        int count = 1;

        fp = fopen(target[z], "r");
        while (1) {
            if (fscanf(fp, " KEY = %s", input_str) == EOF) break;
            *keyLen = strlen(input_str) / 2;
            for (int i = 0;i < *keyLen;i++) sscanf(input_str + 2 * i, "%2hhx", key + i);
            keySchedule(*keyLen, key, encKey, decKey);

            fscanf(fp, " IV = %s", input_str);
            for (int i = 0;i < 16;i++) sscanf(input_str + 2 * i, "%2hhx", IV + i);

            fscanf(fp, " PT = %s", input_str);
            *plainLen = strlen(input_str) / 2;
            for (int i = 0;i < *plainLen;i++) sscanf(input_str + 2 * i, "%2hhx", plain + i);

            CBCenc(*plainLen, plain, IV, *keyLen, encKey, cipherLen, cipher);

            fscanf(fp, " CT = %s", input_str);
            for (int i = 0;i < *cipherLen;i++) sscanf(input_str + 2 * i, "%2hhx", compare + i);

            printf("%dth test : ", count);
            for (int i = 0;i < *cipherLen;i++) printf("%02X", cipher[i]);
            for (int i = 0;i < *cipherLen;i++) {
                if (cipher[i] != compare[i]) {
                    printf(" Fail!");
                    return;
                }
            }
            printf(" Success\n");
            count += 1;
        }
        printf("%s complete\n\n", target[z]);
        isSuccess[z] = 1;
        fclose(fp);
    }
    printf("LEA-128-CBC KAT: %s, LEA-192-CBC KAT: %s, LEA-256-CBC KAT: %s", isSuccess[0] ? "Success" : "Fail", isSuccess[1] ? "Success" : "Fail", isSuccess[2] ? "Success\n" : "Fail\n");
    printf("LEA-128-CBC MMT: %s, LEA-192-CBC MMT: %s, LEA-256-CBC MMT: %s", isSuccess[3] ? "Success" : "Fail", isSuccess[4] ? "Success" : "Fail", isSuccess[5] ? "Success\n" : "Fail\n");
}

void LEA_ALL_CTR_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher) {
    char target[6][150] = { "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CTR)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CTR)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CTR)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CTR)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CTR)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CTR)MMT.txt" };
    int isSuccess[6] = { 0, };
    for (int z = 0;z < 6;z++) {
        FILE* fp;
        char input_str[16 * MAXBLOCK + 1] = { 0 };
        unsigned char compare[16 * MAXBLOCK] = { 0x00, };
        int count = 1;

        fp = fopen(target[z], "r");
        while (1) {
            if (fscanf(fp, " KEY = %s", input_str) == EOF) break;
            *keyLen = strlen(input_str) / 2;
            for (int i = 0;i < *keyLen;i++) sscanf(input_str + 2 * i, "%2hhx", key + i);
            keySchedule(*keyLen, key, encKey, decKey);

            fscanf(fp, " CTR = %s", input_str);
            for (int i = 0;i < 16;i++) sscanf(input_str + 2 * i, "%2hhx", IV + i);

            fscanf(fp, " PT = %s", input_str);
            *plainLen = strlen(input_str) / 2;
            for (int i = 0;i < *plainLen;i++) sscanf(input_str + 2 * i, "%2hhx", plain + i);

            CTRenc(*plainLen, plain, IV, *keyLen, encKey, cipherLen, cipher);

            fscanf(fp, " CT = %s", input_str);
            for (int i = 0;i < *cipherLen;i++) sscanf(input_str + 2 * i, "%2hhx", compare + i);

            printf("%dth test : ", count);
            printf("\n"); for (int i = 0;i < *cipherLen;i++) printf("%02X", cipher[i]);
            for (int i = 0;i < *cipherLen;i++) {
                if (cipher[i] != compare[i]) {
                    printf(" Fail!");
                    return;
                }
            }
            printf(" Success\n");
            count += 1;
        }
        printf("%s complete\n\n", target[z]);
        isSuccess[z] = 1;
        fclose(fp);
    }
    printf("LEA-128-CTR KAT: %s, LEA-192-CTR KAT: %s, LEA-256-CTR KAT: %s", isSuccess[0] ? "Success" : "Fail", isSuccess[1] ? "Success" : "Fail", isSuccess[2] ? "Success\n" : "Fail\n");
    printf("LEA-128-CTR MMT: %s, LEA-192-CTR MMT: %s, LEA-256-CTR MMT: %s", isSuccess[3] ? "Success" : "Fail", isSuccess[4] ? "Success" : "Fail", isSuccess[5] ? "Success\n" : "Fail\n");
}

void LEA_ALL_CFB1_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int s, int* plainLenBit, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLenBit, unsigned char* cipher) {
    char target[6][150] = { "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CFB1)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CFB1)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CFB1)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CFB1)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CFB1)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CFB1)MMT.txt" };
    int isSuccess[6] = { 0, };
    for (int z = 0;z < 6;z++) {
        FILE* fp;
        char input_str[16 * MAXBLOCK + 1] = { 0 };
        unsigned char compare[16 * MAXBLOCK] = { 0x00, };
        int count = 1;

        fp = fopen(target[z], "r");
        while (1) {
            if (fscanf(fp, " KEY = %s", input_str) == EOF) break;
            *keyLen = strlen(input_str) / 2;
            for (int i = 0;i < *keyLen;i++) sscanf(input_str + 2 * i, "%2hhx", key + i);
            keySchedule(*keyLen, key, encKey, decKey);

            fscanf(fp, " IV = %s", input_str);
            for (int i = 0;i < 16;i++) sscanf(input_str + 2 * i, "%2hhx", IV + i);

            fscanf(fp, " PT = %s", input_str);
            *plainLenBit = 1;
            for (int i = 0;i < *plainLenBit;i++) sscanf(input_str + 2 * i, "%2hhx", plain + i);

            CFBenc(1, plain, IV, s, *keyLen, encKey, cipherLenBit, cipher);

            fscanf(fp, " CT = %s", input_str);
            for (int i = 0;i < 1;i++) sscanf(input_str + 2 * i, "%2hhx", compare + i);

            printf("%dth test : ", count);
            for (int i = 8 - *plainLenBit;i < 8;i++) {
                printf("%d", cipher[*plainLenBit / 8] & (1 << (7 - i)));
            }
            for (int i = 0;i < *cipherLenBit;i++) {
                if (cipher[i] != compare[i]) {
                    printf(" Fail!");
                    return;
                }
            }
            printf(" Success\n");
            count += 1;
        }
        printf("%s complete\n\n", target[z]);
        isSuccess[z] = 1;
        fclose(fp);
    }
    printf("LEA-128-CFB1 KAT: %s, LEA-192-CFB1 KAT: %s, LEA-256-CFB1 KAT: %s", isSuccess[0] ? "Success" : "Fail", isSuccess[1] ? "Success" : "Fail", isSuccess[2] ? "Success\n" : "Fail\n");
    printf("LEA-128-CFB1 MMT: %s, LEA-192-CFB1 MMT: %s, LEA-256-CFB1 MMT: %s", isSuccess[3] ? "Success" : "Fail", isSuccess[4] ? "Success" : "Fail", isSuccess[5] ? "Success\n" : "Fail\n");
}

void LEA_ALL_CFB8_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int s, int* plainLenBit, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLenBit, unsigned char* cipher) {
    char target[6][150] = { "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CFB8)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CFB8)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CFB8)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CFB8)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CFB8)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CFB8)MMT.txt" };
    int isSuccess[6] = { 0, };
    for (int z = 0;z < 6;z++) {
        FILE* fp;
        char input_str[16 * MAXBLOCK + 1] = { 0 };
        unsigned char compare[16 * MAXBLOCK] = { 0x00, };
        int count = 1;

        fp = fopen(target[z], "r");
        while (1) {
            if (fscanf(fp, " KEY = %s", input_str) == EOF) break;
            *keyLen = strlen(input_str) / 2;
            for (int i = 0;i < *keyLen;i++) sscanf(input_str + 2 * i, "%2hhx", key + i);
            keySchedule(*keyLen, key, encKey, decKey);

            fscanf(fp, " IV = %s", input_str);
            for (int i = 0;i < 16;i++) sscanf(input_str + 2 * i, "%2hhx", IV + i);

            fscanf(fp, " PT = %s", input_str);
            *plainLenBit = strlen(input_str) * 4;
            for (int i = 0;i < *plainLenBit / 8;i++) sscanf(input_str + 2 * i, "%2hhx", plain + i);

            CFBenc(*plainLenBit, plain, IV, s, *keyLen, encKey, cipherLenBit, cipher);

            fscanf(fp, " CT = %s", input_str);
            for (int i = 0;i < *cipherLenBit / 8;i++) sscanf(input_str + 2 * i, "%2hhx", compare + i);

            printf("%dth test : ", count);
            for (int i = 0;i < *cipherLenBit / 8;i++) printf("%02X", cipher[i]);
            for (int i = 0;i < *cipherLenBit / 8;i++) {
                if (cipher[i] != compare[i]) {
                    printf(" Fail!");
                    return;
                }
            }
            printf(" Success\n");
            count += 1;
        }
        printf("%s complete\n\n", target[z]);
        isSuccess[z] = 1;
        fclose(fp);
    }
    printf("LEA-128-CFB8 KAT: %s, LEA-192-CFB8 KAT: %s, LEA-256-CFB8 KAT: %s", isSuccess[0] ? "Success" : "Fail", isSuccess[1] ? "Success" : "Fail", isSuccess[2] ? "Success\n" : "Fail\n");
    printf("LEA-128-CFB8 MMT: %s, LEA-192-CFB8 MMT: %s, LEA-256-CFB8 MMT: %s", isSuccess[3] ? "Success" : "Fail", isSuccess[4] ? "Success" : "Fail", isSuccess[5] ? "Success\n" : "Fail\n");
}

void LEA_ALL_CFB64_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int s, int* plainLenBit, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLenBit, unsigned char* cipher) {
    char target[6][150] = { "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CFB64)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CFB64)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CFB64)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CFB64)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CFB64)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CFB64)MMT.txt" };
    int isSuccess[6] = { 0, };
    for (int z = 0;z < 6;z++) {
        FILE* fp;
        char input_str[16 * MAXBLOCK + 1] = { 0 };
        unsigned char compare[16 * MAXBLOCK] = { 0x00, };
        int count = 1;

        fp = fopen(target[z], "r");
        while (1) {
            if (fscanf(fp, " KEY = %s", input_str) == EOF) break;
            *keyLen = strlen(input_str) / 2;
            for (int i = 0;i < *keyLen;i++) sscanf(input_str + 2 * i, "%2hhx", key + i);
            keySchedule(*keyLen, key, encKey, decKey);

            fscanf(fp, " IV = %s", input_str);
            for (int i = 0;i < 16;i++) sscanf(input_str + 2 * i, "%2hhx", IV + i);

            fscanf(fp, " PT = %s", input_str);
            *plainLenBit = strlen(input_str) * 4;
            for (int i = 0;i < *plainLenBit / 8;i++) sscanf(input_str + 2 * i, "%2hhx", plain + i);

            CFBenc(*plainLenBit, plain, IV, s, *keyLen, encKey, cipherLenBit, cipher);

            fscanf(fp, " CT = %s", input_str);
            for (int i = 0;i < *cipherLenBit / 8;i++) sscanf(input_str + 2 * i, "%2hhx", compare + i);

            printf("%dth test : ", count);
            for (int i = 0;i < *cipherLenBit / 8;i++) printf("%02X", cipher[i]);
            for (int i = 0;i < *cipherLenBit / 8;i++) {
                if (cipher[i] != compare[i]) {
                    printf(" Fail!");
                    return;
                }
            }
            printf(" Success\n");
            count += 1;
        }
        printf("%s complete\n\n", target[z]);
        isSuccess[z] = 1;
        fclose(fp);
    }
    printf("LEA-128-CFB64 KAT: %s, LEA-192-CFB64 KAT: %s, LEA-256-CFB64 KAT: %s", isSuccess[0] ? "Success" : "Fail", isSuccess[1] ? "Success" : "Fail", isSuccess[2] ? "Success\n" : "Fail\n");
    printf("LEA-128-CFB64 MMT: %s, LEA-192-CFB64 MMT: %s, LEA-256-CFB64 MMT: %s", isSuccess[3] ? "Success" : "Fail", isSuccess[4] ? "Success" : "Fail", isSuccess[5] ? "Success\n" : "Fail\n");
}

void LEA_ALL_CFB128_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int s, int* plainLenBit, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLenBit, unsigned char* cipher) {
    char target[6][150] = { "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CFB128)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CFB128)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CFB128)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CFB128)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CFB128)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CFB128)MMT.txt" };
    int isSuccess[6] = { 0, };
    for (int z = 0;z < 6;z++) {
        FILE* fp;
        char input_str[16 * MAXBLOCK + 1] = { 0 };
        unsigned char compare[16 * MAXBLOCK] = { 0x00, };
        int count = 1;

        fp = fopen(target[z], "r");
        while (1) {
            if (fscanf(fp, " KEY = %s", input_str) == EOF) break;
            *keyLen = strlen(input_str) / 2;
            for (int i = 0;i < *keyLen;i++) sscanf(input_str + 2 * i, "%2hhx", key + i);
            keySchedule(*keyLen, key, encKey, decKey);

            fscanf(fp, " IV = %s", input_str);
            for (int i = 0;i < 16;i++) sscanf(input_str + 2 * i, "%2hhx", IV + i);

            fscanf(fp, " PT = %s", input_str);
            *plainLenBit = strlen(input_str) * 4;
            for (int i = 0;i < *plainLenBit / 8;i++) sscanf(input_str + 2 * i, "%2hhx", plain + i);

            CFBenc(*plainLenBit, plain, IV, s, *keyLen, encKey, cipherLenBit, cipher);

            fscanf(fp, " CT = %s", input_str);
            for (int i = 0;i < *cipherLenBit / 8;i++) sscanf(input_str + 2 * i, "%2hhx", compare + i);

            printf("%dth test : ", count);
            for (int i = 0;i < *cipherLenBit / 8;i++) printf("%02X", cipher[i]);
            for (int i = 0;i < *cipherLenBit / 8;i++) {
                if (cipher[i] != compare[i]) {
                    printf(" Fail!");
                    return;
                }
            }
            printf(" Success\n");
            count += 1;
        }
        printf("%s complete\n\n", target[z]);
        isSuccess[z] = 1;
        fclose(fp);
    }
    printf("LEA-128-CFB128 KAT: %s, LEA-192-CFB128 KAT: %s, LEA-256-CFB128 KAT: %s", isSuccess[0] ? "Success" : "Fail", isSuccess[1] ? "Success" : "Fail", isSuccess[2] ? "Success\n" : "Fail\n");
    printf("LEA-128-CFB128 MMT: %s, LEA-192-CFB128 MMT: %s, LEA-256-CFB128 MMT: %s", isSuccess[3] ? "Success" : "Fail", isSuccess[4] ? "Success" : "Fail", isSuccess[5] ? "Success\n" : "Fail\n");
}

void LEA_ALL_OFB_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher) {
    char target[6][150] = { "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(OFB)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(OFB)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(OFB)KAT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(OFB)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(OFB)MMT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(OFB)MMT.txt" };
    int isSuccess[6] = { 0, };
    for (int z = 0;z < 6;z++) {
        FILE* fp;
        char input_str[16 * MAXBLOCK + 1] = { 0 };
        unsigned char compare[16 * MAXBLOCK] = { 0x00, };
        int count = 1;

        fp = fopen(target[z], "r");
        while (1) {
            *keyLen = strlen(input_str) / 2;
            for (int i = 0;i < *keyLen;i++) sscanf(input_str + 2 * i, "%2hhx", key + i);
            keySchedule(*keyLen, key, encKey, decKey);

            fscanf(fp, " IV = %s", input_str);
            for (int i = 0;i < 16;i++) sscanf(input_str + 2 * i, "%2hhx", IV + i);

            fscanf(fp, " PT = %s", input_str);
            *plainLen = strlen(input_str) / 2;
            for (int i = 0;i < *plainLen;i++) sscanf(input_str + 2 * i, "%2hhx", plain + i);

            OFBenc(*plainLen, plain, IV, *keyLen, encKey, cipherLen, cipher);

            fscanf(fp, " CT = %s", input_str);
            for (int i = 0;i < *cipherLen;i++) sscanf(input_str + 2 * i, "%2hhx", compare + i);

            printf("%dth test : ", count);
            for (int i = 0;i < *cipherLen;i++) printf("%02X", cipher[i]);
            for (int i = 0;i < *cipherLen;i++) {
                if (cipher[i] != compare[i]) {
                    printf(" Fail!");
                    return;
                }
            }
            printf(" Success\n");
            count += 1;
        }
        printf("%s complete\n\n", target[z]);
        isSuccess[z] = 1;
        fclose(fp);
    }
    printf("LEA-128-OFB KAT: %s, LEA-192-OFB KAT: %s, LEA-256-OFB KAT: %s", isSuccess[0] ? "Success" : "Fail", isSuccess[1] ? "Success" : "Fail", isSuccess[2] ? "Success\n" : "Fail\n");
    printf("LEA-128-OFB MMT: %s, LEA-192-OFB MMT: %s, LEA-256-OFB MMT: %s", isSuccess[3] ? "Success" : "Fail", isSuccess[4] ? "Success" : "Fail", isSuccess[5] ? "Success\n" : "Fail\n");
}

void LEA_ALL_ECB_MCT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher) {
    char target[3][150] = { "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(ECB)MCT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(ECB)MCT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(ECB)MCT.txt" };
    int isSuccess[3] = { 0, };
    for (int z = 0;z < 3;z++) {
        FILE* fp;
        char input_str[16 * MAXBLOCK + 1] = { 0 };
        unsigned char compare[16 * MAXBLOCK] = { 0x00, };
        int count = 1;

        fp = fopen(target[z], "r");
        while (1) {
            if (fscanf(fp, " COUNT = %d", &count) == EOF) break;

            fscanf(fp, " KEY = %s", input_str);
            if (count == 0) {
                *keyLen = strlen(input_str) / 2;
                for (int i = 0;i < *keyLen;i++) sscanf(input_str + 2 * i, "%2hhx", key + i);
            }
            keySchedule(*keyLen, key, encKey, decKey);

            fscanf(fp, " PT = %s", input_str);
            if (count == 0) {
                *plainLen = strlen(input_str) / 2;
                for (int i = 0;i < *plainLen;i++) sscanf(input_str + 2 * i, "%2hhx", plain + i);
            }

            unsigned char previous_cipher[16] = { 0, };
            for (int thousand = 0;thousand < 1000;thousand++) {
                ECBenc(*plainLen, plain, *keyLen, encKey, cipherLen, cipher);
                memcpy(plain, cipher, *cipherLen);
                if (thousand == 999) break;
                memcpy(previous_cipher, cipher, *cipherLen);
            }
            if (*keyLen == 16) {
                for (int i = 0;i < *keyLen;i++) key[i] ^= cipher[i];
            }
            else if (*keyLen == 24) {
                for (int i = 0;i < 8;i++) key[i] ^= previous_cipher[i + 8];
                for (int i = 8;i < *keyLen;i++) key[i] ^= cipher[i - 8];
            }
            else if (*keyLen == 32) {
                for (int i = 0;i < 16;i++) key[i] ^= previous_cipher[i];
                for (int i = 16;i < *keyLen;i++) key[i] ^= cipher[i - 16];
            }

            fscanf(fp, " CT = %s", input_str);
            for (int i = 0;i < *cipherLen;i++) sscanf(input_str + 2 * i, "%2hhx", compare + i);

            printf("%dth test : ", count);
            for (int i = 0;i < *cipherLen;i++) printf("%02X", cipher[i]);
            for (int i = 0;i < *cipherLen;i++) {
                if (cipher[i] != compare[i]) {
                    printf(" Fail!");
                    return;
                }
            }
            printf(" Success\n");
            count += 1;
        }
        printf("%s complete\n\n", target[z]);
        isSuccess[z] = 1;
        fclose(fp);
    }
    printf("LEA-128-ECB MCT: %s, LEA-192-ECB MCT: %s, LEA-256-ECB MCT: %s", isSuccess[0] ? "Success" : "Fail", isSuccess[1] ? "Success" : "Fail", isSuccess[2] ? "Success\n" : "Fail\n");
}

void LEA_ALL_CBC_MCT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher) {
    char target[3][150] = { "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CBC)MCT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CBC)MCT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CBC)MCT.txt" };
    int isSuccess[3] = { 0, };
    for (int z = 0;z < 3;z++) {
        FILE* fp;
        char input_str[16 * MAXBLOCK + 1] = { 0 };
        unsigned char compare[16 * MAXBLOCK] = { 0x00, };
        int count = 0;

        fp = fopen(target[z], "r");
        while (1) {
            if (fscanf(fp, " COUNT = %d", &count) == EOF) break;
            fscanf(fp, " KEY = %s", input_str);
            if (count == 0) {
                *keyLen = strlen(input_str) / 2;
                for (int i = 0;i < *keyLen;i++) sscanf(input_str + 2 * i, "%2hhx", key + i);
            }
            keySchedule(*keyLen, key, encKey, decKey);

            fscanf(fp, " IV = %s", input_str);
            for (int i = 0;i < 16;i++) sscanf(input_str + 2 * i, "%2hhx", IV + i);

            fscanf(fp, " PT = %s", input_str);
            if (count == 0) {
                *plainLen = strlen(input_str) / 2;
                for (int i = 0;i < *plainLen;i++) sscanf(input_str + 2 * i, "%2hhx", plain + i);
            }

            unsigned char previous_cipher[16] = { 0, };
            for (int thousand = 0;thousand < 1000;thousand++) {
                if (thousand == 0) {
                    CBCenc(*plainLen, plain, IV, *keyLen, encKey, cipherLen, cipher);
                    memcpy(plain, IV, *plainLen);
                    memcpy(IV, cipher, *plainLen);
                    memcpy(previous_cipher, cipher, *plainLen);
                }
                else {
                    CBCenc(*plainLen, plain, IV, *keyLen, encKey, cipherLen, cipher);
                    memcpy(plain, previous_cipher, *plainLen);
                    memcpy(IV, cipher, *plainLen);
                    if (thousand == 999) break;
                    memcpy(previous_cipher, cipher, *plainLen);
                }
            }
            if (*keyLen == 16) {
                for (int i = 0;i < *keyLen;i++) key[i] ^= cipher[i];
            }
            else if (*keyLen == 24) {
                for (int i = 0;i < 8;i++) key[i] ^= previous_cipher[i + 8];
                for (int i = 8;i < *keyLen;i++) key[i] ^= cipher[i - 8];
            }
            else if (*keyLen == 32) {
                for (int i = 0;i < 16;i++) key[i] ^= previous_cipher[i];
                for (int i = 16;i < *keyLen;i++) key[i] ^= cipher[i - 16];
            }

            fscanf(fp, " CT = %s", input_str);
            for (int i = 0;i < *cipherLen;i++) sscanf(input_str + 2 * i, "%2hhx", compare + i);

            printf("COUNT %d : ", count);
            for (int i = 0;i < *cipherLen;i++) printf("%02X", cipher[i]);
            for (int i = 0;i < *cipherLen;i++) {
                if (cipher[i] != compare[i]) {
                    printf(" Fail!");
                    return;
                }
            }
            printf(" Success\n");
            count += 1;
        }
        printf("%s complete\n\n", target[z]);
        isSuccess[z] = 1;
        fclose(fp);
    }
    printf("LEA-128-CBC MCT: %s, LEA-192-CBC MCT: %s, LEA-256-CBC MCT: %s", isSuccess[0] ? "Success" : "Fail", isSuccess[1] ? "Success" : "Fail", isSuccess[2] ? "Success\n" : "Fail\n");
}

void LEA_ALL_CTR_MCT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain,
    unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher) {
    char target[3][150] = { "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA128(CTR)MCT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA192(CTR)MCT.txt",
                        "C:\\Users\\user\\Desktop\\Study\\code\\CSE\\LEA\\test\\LEA256(CTR)MCT.txt" };
    int isSuccess[3] = { 0, };
    for (int z = 0;z < 3;z++) {
        FILE* fp;
        char input_str[16 * MAXBLOCK + 1] = { 0 };
        unsigned char compare[16 * MAXBLOCK] = { 0x00, };
        int count = 0;

        fp = fopen(target[z], "r");
        while (1) {
            if (fscanf(fp, " COUNT = %d", &count) == EOF) break;
            fscanf(fp, " KEY = %s", input_str);
            if (count == 0) {
                *keyLen = strlen(input_str) / 2;
                for (int i = 0;i < *keyLen;i++) sscanf(input_str + 2 * i, "%2hhx", key + i);
            }
            keySchedule(*keyLen, key, encKey, decKey);

            fscanf(fp, " CTR = %s", input_str);
            for (int i = 0;i < 16;i++) sscanf(input_str + 2 * i, "%2hhx", IV + i);

            fscanf(fp, " PT = %s", input_str);
            if (count == 0) {
                *plainLen = strlen(input_str) / 2;
                for (int i = 0;i < *plainLen;i++) sscanf(input_str + 2 * i, "%2hhx", plain + i);
            }

            unsigned char previous_cipher[16] = { 0, };
            for (int thousand = 0;thousand < 1000;thousand++) {
                CTRenc(*plainLen, plain, IV, *keyLen, encKey, cipherLen, cipher);
                for (int j = 15;j >= 0;j--) {
                    IV[j]++;
                    if (IV[j] != 0) break;
                }
                memcpy(plain, cipher, *plainLen);
                if (thousand == 999) break;
                memcpy(previous_cipher, cipher, *plainLen);
            }
            if (*keyLen == 16) {
                for (int i = 0;i < *keyLen;i++) key[i] ^= cipher[i];
            }
            else if (*keyLen == 24) {
                for (int i = 0;i < 8;i++) key[i] ^= previous_cipher[i + 8];
                for (int i = 8;i < *keyLen;i++) key[i] ^= cipher[i - 8];
            }
            else if (*keyLen == 32) {
                for (int i = 0;i < 16;i++) key[i] ^= previous_cipher[i];
                for (int i = 16;i < *keyLen;i++) key[i] ^= cipher[i - 16];
            }

            fscanf(fp, " CT = %s", input_str);
            for (int i = 0;i < *cipherLen;i++) sscanf(input_str + 2 * i, "%2hhx", compare + i);

            printf("COUNT %d : ", count);
            for (int i = 0;i < *cipherLen;i++) printf("%02X", cipher[i]);
            for (int i = 0;i < *cipherLen;i++) {
                if (cipher[i] != compare[i]) {
                    printf(" Fail!");
                    return;
                }
            }
            printf(" Success\n");
            count += 1;
        }
        printf("%s complete\n\n", target[z]);
        isSuccess[z] = 1;
        fclose(fp);
    }
    printf("LEA-128-CTR MCT: %s, LEA-192-CTR MCT: %s, LEA-256-CTR MCT: %s", isSuccess[0] ? "Success" : "Fail", isSuccess[1] ? "Success" : "Fail", isSuccess[2] ? "Success\n" : "Fail\n");
}
