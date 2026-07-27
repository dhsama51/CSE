#define _CRT_SECURE_NO_WARNINGS
#include "LEAGlobal.h"
#include "LEAEncDec.h"
#include "LEAPad.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void zeroPad(int plainLen, unsigned char* plain, int* paddedLen, unsigned char* padded) {
    int needed = (plainLen % 128) ? 128 - (plainLen % 128) : 0;
    *paddedLen = plainLen + needed;
    int plainLenByte = (plainLen / 8) + (plainLen % 8 ? 1 : 0);
    int paddedLenByte = *paddedLen / 8;
    memcpy(padded, plain, plainLenByte);
    padded[plainLen / 8] &= (unsigned char)(0xff << (-plainLen) & 7);
    memset(padded + plainLenByte, 0x00, paddedLenByte - plainLenByte);
}

void oneAndZerosPad(int plainLen, unsigned char* plain, int* paddedLen, unsigned char* padded) {
    int needed = 128 - (plainLen % 128);
    *paddedLen = plainLen + needed;
    memcpy(padded, plain, plainLen / 8 + (plainLen % 8 ? 1 : 0));

    padded[plainLen / 8] &= (0xff << 8 - plainLen % 8);
    padded[plainLen / 8] |= (unsigned char)(0x80 >> plainLen % 8);
    for (int i = plainLen / 8 + 1;i * 8 < *paddedLen;i++) padded[i] = 0;
}

void unoneAndZerosPad(int* plainLen, unsigned char* plain, int paddedLen, unsigned char* padded) {
    int count = 0;
    while (!(padded[paddedLen / 8 - count / 8 - 1] & (1 << count % 8))) {
        count++;
    }
    *plainLen = paddedLen - count - 1;
    padded[*plainLen / 8] ^= (unsigned char)(0x80 >> (*plainLen % 8));
    memcpy(plain, padded, *plainLen / 8 + (*plainLen % 8 ? 1 : 0));
}

void PKCS5Pad(int plainLen, unsigned char* plain, int* paddedLen, unsigned char* padded) {
    int needed = 16 - (plainLen % 16);
    *paddedLen = plainLen + needed;
    memcpy(padded, plain, plainLen);
    memset(padded + plainLen, (char)needed, needed);
}

void unPKCS5Pad(int* plainLen, unsigned char* plain, int paddedLen, unsigned char* padded) {
    int padLen = padded[paddedLen - 1];
    *plainLen = paddedLen - padLen;
    memcpy(plain, padded, *plainLen);
}
