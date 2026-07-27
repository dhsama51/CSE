#define _CRT_SECURE_NO_WARNINGS
#include "LEAGlobal.h"
#include "LEAEncDec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void keySchedule(int keyLen, unsigned char* key, unsigned int encKey[][6], unsigned int decKey[][6]) {
    unsigned int temp[8] = { 0, };
    switch (keyLen) {
    case 16:
        memcpy(temp, key, 16);
        for (int i = 0;i < 24;i++) {
            temp[0] = ROL(temp[0] + ROL(delta[i % 4], i), 1);
            temp[1] = ROL(temp[1] + ROL(delta[i % 4], i + 1), 3);
            temp[2] = ROL(temp[2] + ROL(delta[i % 4], i + 2), 6);
            temp[3] = ROL(temp[3] + ROL(delta[i % 4], i + 3), 11);
            encKey[i][0] = decKey[23 - i][0] = temp[0]; encKey[i][1] = decKey[23 - i][1] = temp[1];
            encKey[i][2] = decKey[23 - i][2] = temp[2]; encKey[i][3] = decKey[23 - i][3] = temp[1];
            encKey[i][4] = decKey[23 - i][4] = temp[3]; encKey[i][5] = decKey[23 - i][5] = temp[1];
        }
        break;
    case 24:
        memcpy(temp, key, 24);
        for (int i = 0;i < 28;i++) {
            temp[0] = ROL(temp[0] + ROL(delta[i % 6], i), 1);
            temp[1] = ROL(temp[1] + ROL(delta[i % 6], i + 1), 3);
            temp[2] = ROL(temp[2] + ROL(delta[i % 6], i + 2), 6);
            temp[3] = ROL(temp[3] + ROL(delta[i % 6], i + 3), 11);
            temp[4] = ROL(temp[4] + ROL(delta[i % 6], i + 4), 13);
            temp[5] = ROL(temp[5] + ROL(delta[i % 6], i + 5), 17);
            encKey[i][0] = decKey[27 - i][0] = temp[0]; encKey[i][1] = decKey[27 - i][1] = temp[1];
            encKey[i][2] = decKey[27 - i][2] = temp[2]; encKey[i][3] = decKey[27 - i][3] = temp[3];
            encKey[i][4] = decKey[27 - i][4] = temp[4]; encKey[i][5] = decKey[27 - i][5] = temp[5];
        }
        break;
    case 32:
        memcpy(temp, key, 32);
        for (int i = 0;i < 32;i++) {
            temp[(6 * i) % 8] = ROL(temp[(6 * i) % 8] + ROL(delta[i % 8], i), 1);
            temp[(6 * i + 1) % 8] = ROL(temp[(6 * i + 1) % 8] + ROL(delta[i % 8], i + 1), 3);
            temp[(6 * i + 2) % 8] = ROL(temp[(6 * i + 2) % 8] + ROL(delta[i % 8], i + 2), 6);
            temp[(6 * i + 3) % 8] = ROL(temp[(6 * i + 3) % 8] + ROL(delta[i % 8], i + 3), 11);
            temp[(6 * i + 4) % 8] = ROL(temp[(6 * i + 4) % 8] + ROL(delta[i % 8], i + 4), 13);
            temp[(6 * i + 5) % 8] = ROL(temp[(6 * i + 5) % 8] + ROL(delta[i % 8], i + 5), 17);
            encKey[i][0] = decKey[31 - i][0] = temp[(6 * i) % 8];     encKey[i][1] = decKey[31 - i][1] = temp[(6 * i + 1) % 8];
            encKey[i][2] = decKey[31 - i][2] = temp[(6 * i + 2) % 8]; encKey[i][3] = decKey[31 - i][3] = temp[(6 * i + 3) % 8];
            encKey[i][4] = decKey[31 - i][4] = temp[(6 * i + 4) % 8]; encKey[i][5] = decKey[31 - i][5] = temp[(6 * i + 5) % 8];
        }
        break;
    default:
        printf("key size err. Not 128/192/256bit");
        exit(-1);
    }
}

void encRound(unsigned int plain[], unsigned int encKey[]) {
    unsigned int temp[4] = { 0x00, };
    temp[0] = ROL((plain[0] ^ encKey[0]) + (plain[1] ^ encKey[1]), 9);
    temp[1] = ROR((plain[1] ^ encKey[2]) + (plain[2] ^ encKey[3]), 5);
    temp[2] = ROR((plain[2] ^ encKey[4]) + (plain[3] ^ encKey[5]), 3);
    temp[3] = plain[0];
    memcpy(plain, temp, 16);
}

void decRound(unsigned int cipher[], unsigned int decKey[]) {
    unsigned int temp[4] = { 0x00, };
    temp[0] = cipher[3];
    temp[1] = (ROR(cipher[0], 9) - (temp[0] ^ decKey[0])) ^ decKey[1];
    temp[2] = (ROL(cipher[1], 5) - (temp[1] ^ decKey[2])) ^ decKey[3];
    temp[3] = (ROL(cipher[2], 3) - (temp[2] ^ decKey[4])) ^ decKey[5];
    memcpy(cipher, temp, 16);
}

void encrypt(unsigned char plain[], int keyLen, unsigned int encKey[][6], unsigned char cipher[]) {
    unsigned int temp[4] = { 0x00, };
    memcpy(temp, plain, 16);
    switch (keyLen) {
    case 16:
        for (int i = 0;i < 24;i++) encRound(temp, encKey[i]);
        break;
    case 24:
        for (int i = 0;i < 28;i++) encRound(temp, encKey[i]);
        break;
    case 32:
        for (int i = 0;i < 32;i++) encRound(temp, encKey[i]);
        break;
    default:
        printf("key size err. Not 128/192/256bit\n");
        exit(-1);
    }
    memcpy(cipher, temp, 16);
}

void decrypt(unsigned char cipher[], int keyLen, unsigned int decKey[][6], unsigned char plain[]) {
    unsigned int temp[4] = { 0x00, };
    memcpy(temp, cipher, 16);
    switch (keyLen) {
    case 16:
        for (int i = 0;i < 24;i++) decRound(temp, decKey[i]);
        break;
    case 24:
        for (int i = 0;i < 28;i++) decRound(temp, decKey[i]);
        break;
    case 32:
        for (int i = 0;i < 32;i++) decRound(temp, decKey[i]);
        break;
    default:
        printf("key size err. Not 128/192/256bi\nt");
        exit(-1);
    }
    memcpy(plain, temp, 16);
}
