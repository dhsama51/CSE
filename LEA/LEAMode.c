#define _CRT_SECURE_NO_WARNINGS
#include "LEAGlobal.h"
#include "LEAEncDec.h"
#include "LEAPad.h"
#include "LEAMode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ECBenc(int plainLen, unsigned char* plain, int keyLen, unsigned int encKey[][6], int* cipherLen, unsigned char* cipher) {
    *cipherLen = plainLen;
    int paddedLen = plainLen + 16;
    unsigned char* padded = (unsigned char*)malloc(paddedLen);
    zeroPad(plainLen * 8, plain, &paddedLen, padded); paddedLen /= 8;

    for (int i = 0; i < paddedLen / 16; i++) encrypt(padded + 16 * i, keyLen, encKey, cipher + 16 * i);
    free(padded);
}

void ECBdec(int* plainLen, unsigned char* plain, int keyLen, unsigned int decKey[][6], int cipherLen, unsigned char* cipher) {
    *plainLen = cipherLen;
    unsigned char* padded = (unsigned char*)malloc(cipherLen);

    for (int i = 0; i < cipherLen / 16; i++) decrypt(cipher + 16 * i, keyLen, decKey, padded + 16 * i);
    //복호화되는 평문과 패딩 값의 구분이 모호 -> unzeroPad 생략
    memcpy(plain, padded, cipherLen);
    free(padded);
}

void CBCenc(int plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][6], int* cipherLen, unsigned char* cipher) {
    *cipherLen = plainLen + 16;
    unsigned char* padded = (unsigned char*)malloc(*cipherLen);
    unsigned char temp[16] = { 0x00, };
    zeroPad(plainLen * 8, plain, cipherLen, padded); *cipherLen /= 8;

    memcpy(temp, IV, 16);
    for (int i = 0;i < *cipherLen / 16;i++) {
        for (int j = 0;j < 16;j++) temp[j] = temp[j] ^ padded[16 * i + j];
        encrypt(temp, keyLen, encKey, cipher + 16 * i);
        memcpy(temp, cipher + 16 * i, 16);
    }
    free(padded);
}

void CBCdec(int* plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int decKey[][6], int cipherLen, unsigned char* cipher) {
    decrypt(cipher, keyLen, decKey, plain);
    for (int i = 0;i < 16;i++) plain[i] = plain[i] ^ IV[i];
    for (int i = 1;i < cipherLen / 16;i++) {
        decrypt(cipher + 16 * i, keyLen, decKey, plain + 16 * i);
        for (int j = 0;j < 16;j++) plain[16 * i + j] = plain[16 * i + j] ^ cipher[16 * i + j - 16];
    }
    //복호화되는 평문과 패딩 값의 구분이 모호 -> unzeroPad 생략
    *plainLen = cipherLen;
}

void leftShiftNbitIn16byte(unsigned char* input, int N) {
    memmove(input, input + (N / 8), 16 - (N / 8));
    memset(input + 16 - (N / 8), 0, N / 8);

    if (N % 8 == 0) return;
    for (int i = 0;i < 15;i++) {
        input[i] = (input[i] << (N % 8)) | (input[i + 1] >> (8 - (N % 8)));
    }
    input[15] <<= (N % 8);
}

void rightShiftNbitIn16byte(unsigned char* input, int N) {
    memmove(input + (N / 8), input, 16 - (N / 8));
    memset(input, 0, N / 8);

    if (N % 8 == 0) return;
    for (int i = 15;i >= 1;i--) {
        input[i] = (input[i] >> (N % 8)) | (input[i - 1] << (8 - (N % 8)));
    }
    input[0] >>= (N % 8);
}

void CFBenc(int plainLenBit, unsigned char* plain, unsigned char* IV, int s, int keyLen, unsigned int encKey[][6], int* cipherLenBit, unsigned char* cipher) {
    memset(cipher, 0, plainLenBit / 8);
    *cipherLenBit = plainLenBit;
    unsigned char temp[16] = { 0x00, };
    unsigned char plainBlock[16] = { 0x00, };
    unsigned char inputBlock[16] = { 0x00, };
    memcpy(inputBlock, IV, 16);

    for (int i = 0;i < plainLenBit;i += s) {
        encrypt(inputBlock, keyLen, encKey, temp);

        memcpy(plainBlock, plain + i / 8, 16);
        if (plainLenBit < 8) leftShiftNbitIn16byte(plainBlock, 8 - plainLenBit);

        if (s == 1) leftShiftNbitIn16byte(plainBlock, i % 8);
        for (int j = 0;j < 16;j++) temp[j] = temp[j] ^ plainBlock[j];

        rightShiftNbitIn16byte(temp, 128 - s);
        leftShiftNbitIn16byte(inputBlock, s);
        for (int j = 0;j < 16;j++) inputBlock[j] |= temp[j];

        leftShiftNbitIn16byte(temp, 128 - i % 128 - s);
        cipher[i / 8] |= temp[(i / 8) % 16];
        for (int j = 1;j < s / 8;j++) cipher[i / 8 + j] |= temp[(i / 8) % 16 + j];
    }
    if (plainLenBit < 8) rightShiftNbitIn16byte(cipher, 8 - plainLenBit);
}

void CFBdec(int* plainLenBit, unsigned char* plain, unsigned char* IV, int s, int keyLen, unsigned int encKey[][6], int cipherLenBit, unsigned char* cipher) {
    memset(plain, 0, cipherLenBit / 8);
    *plainLenBit = cipherLenBit;
    unsigned char temp[16] = { 0x00, };
    unsigned char cipherBlock[16] = { 0x00, };
    unsigned char inputBlock[16] = { 0x00, };
    memcpy(inputBlock, IV, 16);

    for (int i = 0;i < cipherLenBit;i += s) {
        encrypt(inputBlock, keyLen, encKey, temp);

        memcpy(cipherBlock, cipher + i / 8, 16);
        if (cipherLenBit < 8) leftShiftNbitIn16byte(cipherBlock, 8 - cipherLenBit);

        if (s != 128) leftShiftNbitIn16byte(cipherBlock, i % 128);
        for (int j = 0;j < 16;j++) temp[j] = temp[j] ^ cipherBlock[j];

        rightShiftNbitIn16byte(cipherBlock, 128 - s);
        leftShiftNbitIn16byte(inputBlock, s);
        for (int j = 0;j < 16;j++) inputBlock[j] |= cipherBlock[j];

        rightShiftNbitIn16byte(temp, 128 - s);
        leftShiftNbitIn16byte(temp, 128 - i % 128 - s);
        plain[i / 8] |= temp[(i / 8) % 16];
        for (int j = 1;j < s / 8;j++) plain[i / 8 + j] |= temp[(i / 8) % 16 + j];
    }
}

void OFBenc(int plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][6], int* cipherLen, unsigned char* cipher) {
    *cipherLen = plainLen;
    unsigned char temp[16] = { 0x00, };

    int blockLen = 16;
    memcpy(temp, IV, 16);
    for (int i = 0;i < plainLen;i += 16) {
        encrypt(temp, keyLen, encKey, temp);
        blockLen = (plainLen - i < blockLen) ? plainLen - i : blockLen;
        for (int j = 0;j < blockLen;j++) cipher[i + j] = temp[j] ^ plain[i + j];
    }
}

void OFBdec(int* plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][6], int cipherLen, unsigned char* cipher) {
    *plainLen = cipherLen;
    unsigned char temp[16] = { 0x00, };

    int blockLen = 16;
    memcpy(temp, IV, 16);
    for (int i = 0;i < cipherLen;i += 16) {
        encrypt(temp, keyLen, encKey, temp);
        blockLen = (cipherLen - i < blockLen) ? cipherLen - i : blockLen;
        for (int j = 0;j < blockLen;j++) plain[i + j] = temp[j] ^ cipher[i + j];
    }
}

void CTRenc(int plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][6], int* cipherLen, unsigned char* cipher) {
    *cipherLen = plainLen;
    unsigned char temp[16] = { 0x00, };
    unsigned char* pre_cal = (unsigned char*)malloc(*cipherLen);

    int blockLen = 16;
    memcpy(temp, IV, 16);
    for (int i = 0;i < plainLen;i += 16) {
        encrypt(temp, keyLen, encKey, pre_cal + i);
        for (int j = 15;j >= 0;j--) {
            temp[j]++;
            if (temp[j] != 0) break;
        }
    }

    for (int i = 0;i < plainLen;i += 16) {
        blockLen = (plainLen - i < blockLen) ? plainLen - i : blockLen;
        for (int j = 0;j < blockLen;j++) cipher[i + j] = pre_cal[i + j] ^ plain[i + j];
    }
}

void CTRdec(int* plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][6], int cipherLen, unsigned char* cipher) {
    *plainLen = cipherLen;
    unsigned char temp[16] = { 0x00, };

    int blockLen = 16;
    memcpy(temp, IV, 16);
    for (int i = 0;i < cipherLen;i += 16) {
        encrypt(temp, keyLen, encKey, plain + i);
        blockLen = (cipherLen - i < blockLen) ? cipherLen - i : blockLen;
        for (int j = 0;j < blockLen;j++) plain[i + j] = plain[i + j] ^ cipher[i + j];
        for (int j = 15;j >= 0;i--) {
            temp[j]++;
            if (temp[j] != 0) break;
        }
    }
}
