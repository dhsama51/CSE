#define _CRT_SECURE_NO_WARNINGS
#include "LEAGlobal.h"
#include "LEAEncDec.h"
#include "LEAPad.h"
#include "LEAMode.h"
#include "LEAMOVS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encDecTest() {
    unsigned char plain[16] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
    unsigned char cipher[16] = { 0x00, };
    int keyLen = 16;
    unsigned char key[32] = { 0x0f, 0x1e, 0x2d, 0x3c, 0x4b, 0x5a, 0x69, 0x78, 0x87, 0x96, 0xa5, 0xb4, 0xc3, 0xd2, 0xe1, 0xf0 };
    unsigned int encKey[32][6] = { 0x00, };
    unsigned int decKey[32][6] = { 0x00, };

    keySchedule(keyLen, key, encKey, decKey);
    encrypt(plain, keyLen, encKey, cipher);
    printf("\n");
    decrypt(cipher, keyLen, decKey, plain);
}

void padTest() {
    int plainLen = 6;
    unsigned char plain[6] = { 0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6 };
    int paddedLen = 32;
    unsigned char padded[32] = { 0x00, };
    memset(padded + plainLen, 0xff, paddedLen - plainLen);

    printf("plain: %dbytes, %dbits\n", plainLen, plainLen * 8);
    for (int i = 0;i < plainLen;i++) {
        printf("%02X", plain[i]);
        if (i == 15) printf(" ");
    }

    zeroPad(plainLen * 8, plain, &paddedLen, padded);
    printf("\n\nAfter zeroPad: %dbits\n", paddedLen);
    for (int i = 0;i < paddedLen / 8;i++) {
        printf("%02X", ((unsigned char*)padded)[i]);
        if (i == 15) printf(" ");
    }

    oneAndZerosPad(plainLen * 8, plain, &paddedLen, padded);
    printf("\n\nAfter oneAndZerosPad: %dbits\n", paddedLen);
    for (int i = 0;i < paddedLen / 8;i++) {
        printf("%02X", ((unsigned char*)padded)[i]);
        if (i == 15) printf(" ");
    }

    unoneAndZerosPad(&plainLen, plain, paddedLen, padded);
    printf("\nAfter unoneAndZerospad: %dbits\n", plainLen);
    for (int i = 0;i < plainLen / 8;i++) {
        printf("%02X", ((unsigned char*)plain)[i]);
        if (i == 15) printf(" ");
    }

    PKCS5Pad(plainLen / 8, plain, &paddedLen, padded);
    printf("\n\nAfter PKCS5Pad: %dbits\n", paddedLen);
    for (int i = 0;i < paddedLen;i++) {
        printf("%02X", ((unsigned char*)padded)[i]);
        if (i == 15) printf(" ");
    }

    unPKCS5Pad(&plainLen, plain, paddedLen, padded);
    printf("\nAfter unPKCS5Pad: %dbits\n", plainLen);
    for (int i = 0;i < plainLen;i++) {
        printf("%02X", ((unsigned char*)plain)[i]);
        if (i == 15) printf(" ");
    }
}

void modeTest() {
    int plainLen = 32;
    unsigned char plain[32 * 40] = { 0x00, };
    int IVLen = 16;
    unsigned char IV[16] = { 0x00, };
    int cipherLen = 0; //암호화 함수 내에서 계산됨
    unsigned char cipher[16 * 40] = { 0x00, };
    int keyLen = 16;
    unsigned char key[32] = { 0x00 }; //32byte로 선언한 뒤 keyLen만큼만 접근
    unsigned int encKey[32][6] = { 0x00, };
    unsigned int decKey[32][6] = { 0x00, };

    char keyInput[] = "AE38ECC785CC238F263D14285216B406";
    char plainInput[] = "F3F7057F5670F3E8BB9D9AAA95F12F71EA30FAB7622F0A9F9EDC2821CA7D0968";
    char IVInput[] = "BB0F694719D4BF967A085D4FD98A37E3";

    for (int i = 0;i < keyLen;i++) sscanf(keyInput + 2 * i, "%2hhx", key + i);
    for (int i = 0;i < plainLen;i++) sscanf(plainInput + 2 * i, "%2hhx", plain + i);
    keySchedule(keyLen, key, encKey, decKey);
    for (int i = 0;i < 16;i++) sscanf(IVInput + 2 * i, "%2hhx", IV + i);

    //ECBenc(plainLen, plain, keyLen, encKey, &cipherLen, cipher);
    //ECBdec(&plainLen, plain, keyLen, decKey, cipherLen, cipher);
    //CBCenc(plainLen, plain, IV, keyLen, encKey, &cipherLen, cipher);
    //CBCdec(&plainLen, plain, IV, keyLen, decKey, cipherLen, cipher);
    //CTRenc(plainLen, plain, IV, keyLen, encKey, &cipherLen, cipher);
    //CTRdec(&plainLen, plain, IV, keyLen, encKey, cipherLen, cipher);
    //int s = 128;
    //CFBenc(plainLen, plain, IV, s, keyLen, encKey, &cipherLen, cipher);
    //CFBdec(&plainLen, plain, IV, s, keyLen, encKey, cipherLen, cipher);
    OFBenc(plainLen, plain, IV, keyLen, encKey, &cipherLen, cipher);
    OFBdec(&plainLen, plain, IV, keyLen, encKey, cipherLen, cipher);
    printf("key(%dbytes)\n", keyLen); for (int i = 0;i < keyLen;i++) printf("%02X", key[i]);
    printf("\nplaintext(%dbytes)\n", plainLen); for (int i = 0;i < plainLen;i++) printf("%02X", plain[i]);
    printf("\nciphertext(%dbytes)\n", cipherLen); for (int i = 0;i < cipherLen;i++) printf("%02X", cipher[i]);
}

void MOVSTest() {
    int keyLen = 16; //입력받는 키 길이는 16/24/32byte. 일단 128bit 디폴트
    unsigned char key[32] = { 0x00 }; //16/24/32byte 사용
    int IVLen = 16;
    unsigned char IV[16] = { 0x00, };
    int plainLen = 16;
    unsigned char plain[16 * MAXBLOCK] = { 0x00, };

    unsigned int encKey[32][6] = { 0x00, }; //24byte를 24/28/32라운드에 사용
    unsigned int decKey[32][6] = { 0x00, }; //24byte를 24/28/32라운드에 사용
    int cipherLen = 32;
    unsigned char cipher[16 * 40] = { 0x00, };

    //LEA_ALL_ECB_KAT_MMT(&keyLen, key, IV, &plainLen, plain, encKey, decKey, &cipherLen, cipher);
    //LEA_ALL_CBC_KAT_MMT(&keyLen, key, IV, &plainLen, plain, encKey, decKey, &cipherLen, cipher);
    //LEA_ALL_CTR_KAT_MMT(&keyLen, key, IV, &plainLen, plain, encKey, decKey, &cipherLen, cipher);
    //LEA_ALL_CFB1_KAT_MMT(&keyLen, key, IV, 1, &plainLen, plain, encKey, decKey, &cipherLen, cipher);
    //LEA_ALL_CFB8_KAT_MMT(&keyLen, key, IV, 8, &plainLen, plain, encKey, decKey, &cipherLen, cipher);
    //LEA_ALL_CFB64_KAT_MMT(&keyLen, key, IV, 64, &plainLen, plain, encKey, decKey, &cipherLen, cipher);
    //LEA_ALL_CFB128_KAT_MMT(&keyLen, key, IV, 128, &plainLen, plain, encKey, decKey, &cipherLen, cipher);
    //LEA_ALL_OFB_KAT_MMT(&keyLen, key, IV, &plainLen, plain, encKey, decKey, &cipherLen, cipher);

    //LEA_ALL_ECB_MCT(&keyLen, key, IV, &plainLen, plain, encKey, decKey, &cipherLen, cipher);
    //LEA_ALL_CBC_MCT(&keyLen, key, IV, &plainLen, plain, encKey, decKey, &cipherLen, cipher);
    LEA_ALL_CTR_MCT(&keyLen, key, IV, &plainLen, plain, encKey, decKey, &cipherLen, cipher);
}

int main() {
    //encDecTest();
    //padTest();
    //modeTest();
    MOVSTest();

    return 0;
}