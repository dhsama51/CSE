#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBLOCK 1000
#define ROL(x, n) (((x) << (n)) | ((x) >> 32 - (n)))
#define ROR(x, n) (((x) >> (n)) | ((x) << 32 - (n)))

unsigned int delta[8] = { 0xc3efe9db, 0x44626b02, 0x79e27c8a, 0x78df30ec,
                        0x715ea49e, 0xc785da0a, 0xe04ef22a, 0xe5c40957 };

void keySchedule(int keyLen, unsigned char* key, unsigned int encKey[][8], unsigned int decKey[][8]) {
    unsigned int temp[8] = { 0, };
    switch (keyLen) {
    case 16:
        memcpy(temp, key, 16);
        for (int i = 0;i < 24;i++) {
            temp[0] = ROL(temp[0] + ROL(delta[i % 4], i), 1); //오버플로우 때문에 mod 2^32 불필요
            temp[1] = ROL(temp[1] + ROL(delta[i % 4], i + 1), 3);
            temp[2] = ROL(temp[2] + ROL(delta[i % 4], i + 2), 6);
            temp[3] = ROL(temp[3] + ROL(delta[i % 4], i + 3), 11);
            encKey[i][0] = temp[0]; encKey[i][1] = temp[1]; encKey[i][2] = temp[2];
            encKey[i][3] = temp[1]; encKey[i][4] = temp[3]; encKey[i][5] = temp[1];
            decKey[23 - i][0] = temp[0]; decKey[23 - i][1] = temp[1]; decKey[23 - i][2] = temp[2];
            decKey[23 - i][3] = temp[1]; decKey[23 - i][4] = temp[3]; decKey[23 - i][5] = temp[1];
        }
        /*printf("<LEA-128 encKey schedule result>\n");
        for(int i=0;i<24;i++) {
            printf("enc %d\t", i);
            printf("%08x ", encKey[i][0]);
            printf("%08x ", encKey[i][1]);
            printf("%08x ", encKey[i][2]);
            printf("%08x ", encKey[i][3]);
            printf("%08x ", encKey[i][4]);
            printf("%08x\n", encKey[i][5]);
        }*/
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
            encKey[i][0] = temp[0]; encKey[i][1] = temp[1]; encKey[i][2] = temp[2];
            encKey[i][3] = temp[3]; encKey[i][4] = temp[4]; encKey[i][5] = temp[5];
            decKey[27 - i][0] = temp[0]; decKey[27 - i][1] = temp[1]; decKey[27 - i][2] = temp[2];
            decKey[27 - i][3] = temp[3]; decKey[27 - i][4] = temp[4]; decKey[27 - i][5] = temp[5];
        }
        /*printf("<LEA-192 encKey schedule result>\n");
        for(int i=0;i<28;i++) {
            printf("enc %d\t", i);
            printf("%08x ", encKey[i][0]);
            printf("%08x ", encKey[i][1]);
            printf("%08x ", encKey[i][2]);
            printf("%08x ", encKey[i][3]);
            printf("%08x ", encKey[i][4]);
            printf("%08x\n", encKey[i][5]);
        }*/
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
            encKey[i][0] = temp[(6 * i) % 8]; encKey[i][1] = temp[(6 * i + 1) % 8]; encKey[i][2] = temp[(6 * i + 2) % 8];
            encKey[i][3] = temp[(6 * i + 3) % 8]; encKey[i][4] = temp[(6 * i + 4) % 8]; encKey[i][5] = temp[(6 * i + 5) % 8];
            decKey[31 - i][0] = temp[(6 * i) % 8]; decKey[31 - i][1] = temp[(6 * i + 1) % 8]; decKey[31 - i][2] = temp[(6 * i + 2) % 8];
            decKey[31 - i][3] = temp[(6 * i + 3) % 8]; decKey[31 - i][4] = temp[(6 * i + 4) % 8]; decKey[31 - i][5] = temp[(6 * i + 5) % 8];
        }
        /*printf("<LEA-256 encKey schedule result>\n");
        for(int i=0;i<32;i++) {
            printf("enc %d\t", i);
            printf("%08x ", encKey[i][0]);
            printf("%08x ", encKey[i][1]);
            printf("%08x ", encKey[i][2]);
            printf("%08x ", encKey[i][3]);
            printf("%08x ", encKey[i][4]);
            printf("%08x\n", encKey[i][5]);
        }*/
        break;
    default:
        printf("key size err. Not 128/192/256bit");
        exit(-1);
    }
}

void encRound(unsigned int plain[4], unsigned int encKey[]) {
    unsigned int temp[4] = { 0x00, };
    temp[0] = ROL((plain[0] ^ encKey[0]) + (plain[1] ^ encKey[1]), 9);
    temp[1] = ROR((plain[1] ^ encKey[2]) + (plain[2] ^ encKey[3]), 5);
    temp[2] = ROR((plain[2] ^ encKey[4]) + (plain[3] ^ encKey[5]), 3);
    temp[3] = plain[0];
    memcpy(plain, temp, 16);
}

void decRound(unsigned int cipher[4], unsigned int decKey[]) {
    unsigned int temp[4] = { 0x00, };
    temp[0] = cipher[3];
    temp[1] = (ROR(cipher[0], 9) - (temp[0] ^ decKey[0])) ^ decKey[1];
    temp[2] = (ROL(cipher[1], 5) - (temp[1] ^ decKey[2])) ^ decKey[3];
    temp[3] = (ROL(cipher[2], 3) - (temp[2] ^ decKey[4])) ^ decKey[5];
    memcpy(cipher, temp, 16);
}

void encrypt(unsigned char plain[], int keyLen, unsigned int encKey[][8], unsigned char cipher[]) {
    unsigned int temp[4] = { 0x00, };
    memcpy(temp, plain, 16);
    switch (keyLen) {
    case 16:
        for (int i = 0;i < 24;i++) {
            /*printf("X%d\t", i);
            printf("%08x ", temp[0]); printf("%08x ", temp[1]);
            printf("%08x ", temp[2]); printf("%08x\n", temp[3]);*/
            encRound(temp, (unsigned int*)encKey[i]);
        }
        /*printf("X24\t");
        printf("%08x ", temp[0]); printf("%08x ", temp[1]);
        printf("%08x ", temp[2]); printf("%08x\n", temp[3]);*/

        memcpy(cipher, temp, 16);
        break;
    case 24:
        for (int i = 0;i < 28;i++) {
            /*printf("X%d\t", i);
            printf("%08x ", temp[0]); printf("%08x ", temp[1]);
            printf("%08x ", temp[2]); printf("%08x\n", temp[3]);*/
            encRound(temp, encKey[i]);
        }
        /*printf("X28\t");
        printf("%08x ", temp[0]); printf("%08x ", temp[1]);
        printf("%08x ", temp[2]); printf("%08x\n", temp[3]);*/

        memcpy(cipher, temp, 16);
        break;
    case 32:
        for (int i = 0;i < 32;i++) {
            /*printf("X%d\t", i);
            printf("%08x ", temp[0]); printf("%08x ", temp[1]);
            printf("%08x ", temp[2]); printf("%08x\n", temp[3]);*/
            encRound(temp, encKey[i]);
        }
        /*printf("X32\t");
        printf("%08x ", temp[0]); printf("%08x ", temp[1]);
        printf("%08x ", temp[2]); printf("%08x\n", temp[3]);*/

        memcpy(cipher, temp, 16);
        break;
    default:
        printf("key size err. Not 128/192/256bi\nt");
        exit(-1);
    }
}

void decrypt(unsigned char cipher[], int keyLen, unsigned int decKey[][8], unsigned char plain[]) {
    unsigned int temp[4] = { 0x00, };
    memcpy(temp, cipher, 16);
    switch (keyLen) {
    case 16:
        for (int i = 0;i < 24;i++) {
            /*printf("X%d\t", i);
            printf("%08x ", temp[0]); printf("%08x ", temp[1]);
            printf("%08x ", temp[2]); printf("%08x\n", temp[3]);*/
            decRound(temp, decKey[i]);
        }
        /*printf("X24\t");
        printf("%08x ", temp[0]); printf("%08x ", temp[1]);
        printf("%08x ", temp[2]); printf("%08x\n", temp[3]);*/

        memcpy(plain, temp, 16);
        break;
    case 24:
        for (int i = 0;i < 28;i++) {
            printf("X%d\t", i);
            printf("%08x ", temp[0]); printf("%08x ", temp[1]);
            printf("%08x ", temp[2]); printf("%08x\n", temp[3]);
            decRound(temp, decKey[i]);
        }
        /*printf("X28\t");
        printf("%08x ", temp[0]); printf("%08x ", temp[1]);
        printf("%08x ", temp[2]); printf("%08x\n", temp[3]);*/

        memcpy(plain, temp, 16);
        break;
    case 32:
        for (int i = 0;i < 32;i++) {
            /*printf("X%d\t", i);
            printf("%08x ", temp[0]); printf("%08x ", temp[1]);
            printf("%08x ", temp[2]); printf("%08x\n", temp[3]);*/
            decRound(temp, decKey[i]);
        }
        /*printf("X32\t");
        printf("%08x ", temp[0]); printf("%08x ", temp[1]);
        printf("%08x ", temp[2]); printf("%08x\n", temp[3]);*/

        memcpy(plain, temp, 16);
        break;
    default:
        printf("key size err. Not 128/192/256bi\nt");
        exit(-1);
    }
}

void encDecTest() {
    //4word=16byte=128bit, 6word=24byte=192bit, 8word=32byte=256bit
    unsigned char plain[16] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
    unsigned char cipher[16] = { 0x00, };
    int keyLen = 16; //입력받는 키 길이는 16/24/32byte. 일단 128bit 디폴트
    unsigned char key[32] = { 0x0f, 0x1e, 0x2d, 0x3c, 0x4b, 0x5a, 0x69, 0x78, 0x87, 0x96, 0xa5, 0xb4, 0xc3, 0xd2, 0xe1, 0xf0, 0xf0, 0xe1, 0xd2, 0xc3, 0xb4, 0xa5, 0x96, 0x87, 0x78, 0x69, 0x5a, 0x4b, 0x3c, 0x2d, 0x1e, 0x0f }; //16/24/32byte 사용
    unsigned int encKey[32][8] = { 0x00, }; //24byte를 24/28/32라운드에 사용
    unsigned int decKey[32][8] = { 0x00, }; //24byte를 24/28/32라운드에 사용

    keySchedule(keyLen, key, encKey, decKey);
    encrypt(plain, keyLen, encKey, cipher);
    printf("\n");
    decrypt(cipher, keyLen, decKey, plain);
}

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
    for (int i = 0;i < paddedLen / 8;i++) {
        printf("%02X", ((unsigned char*)padded)[i]);
        if (i == 15) printf(" ");
    }

    oneAndZerosPad(plainLen * 8, plain, &paddedLen, padded);
    printf("\npadded: %dbits\n", paddedLen);
    for (int i = 0;i < paddedLen / 8;i++) {
        printf("%02X", ((unsigned char*)padded)[i]);
        if (i == 15) printf(" ");
    }
    unoneAndZerosPad(&plainLen, plain, paddedLen, padded);
    printf("\nplain: %dbits\n", plainLen);
    for (int i = 0;i < plainLen / 8;i++) {
        printf("%02X", ((unsigned char*)plain)[i]);
        if (i == 15) printf(" ");
    }

    PKCS5Pad(plainLen / 8, plain, &paddedLen, padded);
    printf("\npadded: %dbits\n", paddedLen);
    for (int i = 0;i < paddedLen;i++) {
        printf("%02X", ((unsigned char*)padded)[i]);
        if (i == 15) printf(" ");
    }
    unPKCS5Pad(&plainLen, plain, paddedLen, padded);
    printf("\nplain: %dbits\n", plainLen);
    for (int i = 0;i < plainLen;i++) {
        printf("%02X", ((unsigned char*)plain)[i]);
        if (i == 15) printf(" ");
    }
}

void ECBenc(int plainLen, unsigned char* plain, int keyLen, unsigned int encKey[][8], int* cipherLen, unsigned char* cipher) {
    *cipherLen = plainLen;
    int paddedLen = plainLen + 16;
    unsigned char* padded = (unsigned char*)malloc(paddedLen);

    zeroPad(plainLen, plain, &paddedLen, padded);
    for (int i = 0; i < paddedLen / 16; i++) {
        encrypt(padded + 16 * i, keyLen, encKey, cipher + 16 * i);
    }
    free(padded);

    //for (int i = 0;i < paddedLen;i++) printf("%02X", cipher[i]);
}

void ECBdec(int* plainLen, unsigned char* plain, int keyLen, unsigned int decKey[][8], int cipherLen, unsigned char* cipher) {
    *plainLen = cipherLen;
    unsigned char* padded = (unsigned char*)malloc(cipherLen);

    for (int i = 0; i < cipherLen / 16; i++) {
        decrypt(cipher + 16 * i, keyLen, decKey, padded + 16 * i);
    }
    //zeroPad는 복호화되는 평문과 패딩 값의 구분이 모호할 수 있어서 unzeroPad는 생략
    memcpy(plain, padded, cipherLen);
    free(padded);

    //for (int i = 0;i < cipherLen;i++) printf("%02x", plain[i]);
}

void CBCenc(int plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][8], int* cipherLen, unsigned char* cipher) {
    *cipherLen = plainLen + 16;
    unsigned char* padded = (unsigned char*)malloc(*cipherLen);
    unsigned char temp[16] = { 0x00, };

    /*printf("plaintext\n");
    for (int i = 0;i < plainLen;i++) printf("%02X", plain[i]);*/
    zeroPad(plainLen, plain, cipherLen, padded);
    /*printf("\npadded\n");
    for (int i = 0;i < *cipherLen;i++) printf("%02X", padded[i]);*/
    for (int i = 0;i < 16;i++) temp[i] = padded[i] ^ IV[i];
    encrypt(temp, keyLen, encKey, cipher);
    memcpy(temp, cipher, 16);
    for (int i = 1;i < *cipherLen / 16;i++) {
        for (int j = 0;j < 16;j++) temp[j] = temp[j] ^ padded[16 * i + j];
        encrypt(temp, keyLen, encKey, cipher + 16 * i);
        memcpy(temp, cipher + 16 * i, 16);
    }
    free(padded);
    /*printf("\nciphertext\n");
    for (int i = 0;i < *cipherLen;i++) printf("%02X", cipher[i]);
    printf("\n");*/
}

void CBCdec(int* plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int decKey[][8], int cipherLen, unsigned char* cipher) {
    decrypt(cipher, keyLen, decKey, plain);
    for (int i = 0;i < 16;i++) plain[i] = plain[i] ^ IV[i];
    for (int i = 1;i < cipherLen / 16;i++) {
        decrypt(cipher + 16 * i, keyLen, decKey, plain + 16 * i);
        for (int j = 0;j < 16;j++) plain[16 * i + j] = plain[16 * i + j] ^ cipher[16 * i + j - 16];
    }
    //zeroPad 제거는 생략
    *plainLen = cipherLen;
    /*printf("plaintext\n");
    for (int i = 0;i < cipherLen;i++) printf("%02X", plain[i]);
    printf("\n");*/
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

void CFBenc(int plainLenBit, unsigned char* plain, unsigned char* IV, int s, int keyLen, unsigned int encKey[][8], int* cipherLenBit, unsigned char* cipher) {
    memset(cipher, 0, plainLenBit / 8);
    *cipherLenBit = plainLenBit;
    unsigned char temp[16] = { 0x00, };
    unsigned char plainBlock[16] = { 0x00, };
    unsigned char inputBlock[16] = { 0x00, };
    memcpy(inputBlock, IV, 16);

    /*for (int i = 0;i < plainLenBit / 8;i++) printf("%02X", plain[i]);
    if (plainLenBit % 8 != 0)
        for (int i = 8 - plainLenBit;i < 8;i++) {
            printf("%d", plain[plainLenBit / 8] & (1 << (7 - i)));
        }*/

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

    /*printf("\n");
    for (int j = 0;j < plainLenBit / 8;j++) printf("%02X", cipher[j]);
    if (plainLenBit % 8 != 0)
        for (int i = 8 - plainLenBit;i < 8;i++) {
            printf("%d", cipher[plainLenBit / 8] & (1 << (7 - i)));
        }
    printf("\n");*/
}

void CFBdec(int* plainLenBit, unsigned char* plain, unsigned char* IV, int s, int keyLen, unsigned int encKey[][8], int cipherLenBit, unsigned char* cipher) {
    memset(plain, 0, cipherLenBit / 8);
    *plainLenBit = cipherLenBit;
    unsigned char temp[16] = { 0x00, };
    unsigned char cipherBlock[16] = { 0x00, };
    unsigned char inputBlock[16] = { 0x00, };
    memcpy(inputBlock, IV, 16);

    for (int j = 0;j < cipherLenBit / 8;j++) printf("%02X", cipher[j]);
    if (cipherLenBit % 8 != 0) printf("%02X", cipher[cipherLenBit / 8]);

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

    /*printf("\n");
    for (int j = 0;j < cipherLenBit / 8;j++) printf("%02X", plain[j]);
    if (cipherLenBit % 8 != 0)
        for (int i = 8 - cipherLenBit;i < 8;i++) {
            printf("%d", plain[cipherLenBit / 8] & (1 << (7 - i)));
        }*/
}

void OFBenc(int plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][8], int* cipherLen, unsigned char* cipher) {
    *cipherLen = plainLen;
    unsigned char temp[16] = { 0x00, };

    /*printf("plaintext\n");
    for (int i = 0;i < plainLen;i++) printf("%02X", plain[i]);*/

    int blockLen = 16;
    memcpy(temp, IV, 16);
    for (int i = 0;i < plainLen;i += 16) {
        encrypt(temp, keyLen, encKey, temp);
        blockLen = (plainLen - i < blockLen) ? plainLen - i : blockLen;
        for (int j = 0;j < blockLen;j++) cipher[i + j] = temp[j] ^ plain[i + j];
    }

    /*printf("\nciphertext(%dbytes)\n", *cipherLen);
    for (int i = 0;i < *cipherLen;i++) printf("%02X", cipher[i]);
    printf("\n");*/
}

void OFBdec(int* plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][8], int cipherLen, unsigned char* cipher) {
    *plainLen = cipherLen;
    unsigned char temp[16] = { 0x00, };

    int blockLen = 16;
    memcpy(temp, IV, 16);
    for (int i = 0;i < cipherLen;i += 16) {
        encrypt(temp, keyLen, encKey, temp);
        blockLen = (cipherLen - i < blockLen) ? cipherLen - i : blockLen;
        for (int j = 0;j < blockLen;j++) plain[i + j] = temp[j] ^ cipher[i + j];
    }

    /*printf("plaintext\n");
    for (int i = 0;i < *plainLen;i++) printf("%02X", plain[i]);
    printf("\n");*/
}

void CTRenc(int plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][8], int* cipherLen, unsigned char* cipher) {
    *cipherLen = plainLen;
    unsigned char temp[16] = { 0x00, };

    /*printf("plaintext\n");
    for (int i = 0;i < plainLen;i++) printf("%02X", plain[i]);*/

    int blockLen = 16;
    memcpy(temp, IV, 16);
    for (int i = 0;i < plainLen;i += 16) {
        encrypt(temp, keyLen, encKey, cipher + i);
        blockLen = (plainLen - i < blockLen) ? plainLen - i : blockLen;
        for (int j = 0;j < blockLen;j++) cipher[i + j] = cipher[i + j] ^ plain[i + j];
        for (int j = 15;j >= 0;j--) {
            temp[j]++;
            if (temp[j] != 0) break;
        }
    }

    /*printf("\nciphertext(%dbytes)\n", *cipherLen);
    for (int i = 0;i < *cipherLen;i++) printf("%02X", cipher[i]);
    printf("\n");*/
}

void CTRdec(int* plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][8], int cipherLen, unsigned char* cipher) {
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

    /*printf("plaintext\n");
    for (int i = 0;i < *plainLen;i++) printf("%02X", plain[i]);
    printf("\n");*/
}

void CMACKey(unsigned char* key, unsigned int encKey[][8], int keyLen, unsigned char subKey[][16]) {
    unsigned char zero128[16] = { 0x00, };
    unsigned char temp[16] = { 0x00, };

    encrypt(zero128, keyLen, encKey, temp);
    if (!(temp[0] & 0x80)) {
        leftShiftNbitIn16byte(temp, 1);
        memcpy(subKey[0], temp, 16);
    }
    else {
        leftShiftNbitIn16byte(temp, 1);
        temp[15] ^= 0b10000111;
        memcpy(subKey[0], temp, 16);
    }

    if (!(subKey[0][0] & 0x80)) {
        leftShiftNbitIn16byte(temp, 1);
        memcpy(subKey[1], temp, 16);
    }
    else {
        temp[15] ^= 0b10000111;
        memcpy(subKey[1], temp, 16);
    }
}

void CMAC(int plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned char* key, unsigned int encKey[][8], unsigned char* result) {
    unsigned char* padded = (unsigned char*)malloc(plainLen + 16);
    int paddedLen = 0;
    unsigned char temp[16] = { 0x00, };
    unsigned char subKey[2][16] = { 0x00, };

    CMACKey(key, encKey, keyLen, subKey);
    /*printf("plaintext\n");
    for (int i = 0;i < plainLen;i++) printf("%02X", plain[i]);*/
    if (plainLen % 16 == 0) paddedLen = plainLen;
    else oneAndZerosPad(plainLen, plain, &paddedLen, padded);

    /*printf("\npadded\n");
    for (int i = 0;i < *cipherLen;i++) printf("%02X", padded[i]);*/
    memcpy(temp, padded, 16);
    for (int i = 1;i < paddedLen / 16;i++) {
        encrypt(temp, keyLen, encKey, temp);
        for (int j = 0;j < 16;j++) temp[j] = temp[j] ^ padded[16 * i + j];
    }

    if (plainLen % 16 == 0) for (int i = 0;i < 16;i++) result[i] = temp[i] ^ subKey[0][i];
    else for (int i = 0;i < 16;i++) result[i] = temp[i] ^ subKey[1][i];
    for (int i = 0;i < 16;i++) printf("%02x", result[i]);
    free(padded);
    /*printf("\nciphertext\n");
    for (int i = 0;i < *cipherLen;i++) printf("%02X", cipher[i]);
    printf("\n");*/
}

void modeTest() {
    int plainLen = 16;
    unsigned char plain[32 * 40] = { 0x00, };
    int IVLen = 16;
    unsigned char IV[16] = { 0x00, };
    int cipherLen = 32;
    unsigned char cipher[16 * 40] = { 0x00, };
    int keyLen = 16; //입력받는 키 길이는 16/24/32byte. 일단 128bit 디폴트
    unsigned char key[32] = { 0x00 }; //16/24/32byte 사용
    unsigned int encKey[32][8] = { 0x00, }; //24byte를 24/28/32라운드에 사용
    unsigned int decKey[32][8] = { 0x00, }; //24byte를 24/28/32라운드에 사용

    char keyInput[] = "00000000000000000000000000000000";
    char plainInput[] = "00000000000000000000000000000000";
    char IVInput[] = "80000000000000000000000000000000";

    for (int i = 0;i < keyLen;i++) sscanf(keyInput + 2 * i, "%2hhx", key + i);
    for (int i = 0;i < plainLen;i++) sscanf(plainInput + 2 * i, "%2hhx", plain + i);
    keySchedule(keyLen, key, encKey, decKey);
    for (int i = 0;i < 16;i++) sscanf(IVInput + 2 * i, "%2hhx", IV + i);

    //ECBenc(plainLen, plain, keyLen, encKey, &cipherLen, cipher);
    //ECBdec(&plainLen, plain, keyLen, decKey, cipherLen, cipher);
    //CBCenc(plainLen, plain, IV, keyLen, encKey, &cipherLen, cipher);
    //CBCdec(&plainLen, plain, IV, keyLen, decKey, cipherLen, cipher);
    //int s = 8;
    //CFBenc(plainLen * 8, plain, IV, s, keyLen, encKey, &cipherLen, cipher);
    //CFBdec(&plainLen, plain, IV, s, keyLen, encKey, cipherLen, cipher);
    //OFBenc(plainLen, plain, IV, keyLen, encKey, &cipherLen, cipher);
    //OFBdec(&plainLen, plain, IV, keyLen, encKey, cipherLen, cipher);
    //CTRenc(plainLen, plain, IV, keyLen, encKey, &cipherLen, cipher);
    //CTRdec(&plainLen, plain, IV, keyLen, encKey, cipherLen, cipher);
    //미완성CMAC(plainLen, plain, IV, keyLen, key, encKey, cipher);
}

void LEA_ALL_ECB_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][8], unsigned int decKey[][8], int* cipherLen, unsigned char* cipher) {
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

void LEA_ALL_CBC_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][8], unsigned int decKey[][8], int* cipherLen, unsigned char* cipher) {
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

void LEA_ALL_CTR_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][8], unsigned int decKey[][8], int* cipherLen, unsigned char* cipher) {
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

void LEA_ALL_CFB1_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int s, int* plainLenBit, unsigned char* plain, unsigned int encKey[][8], unsigned int decKey[][8], int* cipherLenBit, unsigned char* cipher) {
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

void LEA_ALL_CFB8_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int s, int* plainLenBit, unsigned char* plain, unsigned int encKey[][8], unsigned int decKey[][8], int* cipherLenBit, unsigned char* cipher) {
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

void LEA_ALL_CFB64_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int s, int* plainLenBit, unsigned char* plain, unsigned int encKey[][8], unsigned int decKey[][8], int* cipherLenBit, unsigned char* cipher) {
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

void LEA_ALL_CFB128_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int s, int* plainLenBit, unsigned char* plain, unsigned int encKey[][8], unsigned int decKey[][8], int* cipherLenBit, unsigned char* cipher) {
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

void LEA_ALL_OFB_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][8], unsigned int decKey[][8], int* cipherLen, unsigned char* cipher) {
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


void LEA_ALL_ECB_MCT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][8], unsigned int decKey[][8], int* cipherLen, unsigned char* cipher) {
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

void LEA_ALL_CBC_MCT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][8], unsigned int decKey[][8], int* cipherLen, unsigned char* cipher) {
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

void LEA_ALL_CTR_MCT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][8], unsigned int decKey[][8], int* cipherLen, unsigned char* cipher) {
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

void MOVS() {
    int keyLen = 16; //입력받는 키 길이는 16/24/32byte. 일단 128bit 디폴트
    unsigned char key[32] = { 0x00 }; //16/24/32byte 사용
    int IVLen = 16;
    unsigned char IV[16] = { 0x00, };
    int plainLen = 16;
    unsigned char plain[16 * MAXBLOCK] = { 0x00, };

    unsigned int encKey[32][8] = { 0x00, }; //24byte를 24/28/32라운드에 사용
    unsigned int decKey[32][8] = { 0x00, }; //24byte를 24/28/32라운드에 사용
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

    LEA_ALL_ECB_MCT(&keyLen, key, IV, &plainLen, plain, encKey, decKey, &cipherLen, cipher);
    //LEA_ALL_CBC_MCT(&keyLen, key, IV, &plainLen, plain, encKey, decKey, &cipherLen, cipher);
    //LEA_ALL_CTR_MCT(&keyLen, key, IV, &plainLen, plain, encKey, decKey, &cipherLen, cipher);
}

int main() {
    //encDecTest();
    padTest();
    //modeTest();
    //MOVS();

    return 0;
}