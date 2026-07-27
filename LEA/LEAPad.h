#ifndef LEAPAD_H
#define LEAPAD_H

void zeroPad(int plainLen, unsigned char* plain, int* paddedLen, unsigned char* padded);
void oneAndZerosPad(int plainLen, unsigned char* plain, int* paddedLen, unsigned char* padded);
void unoneAndZerosPad(int* plainLen, unsigned char* plain, int paddedLen, unsigned char* padded);
void PKCS5Pad(int plainLen, unsigned char* plain, int* paddedLen, unsigned char* padded);
void unPKCS5Pad(int* plainLen, unsigned char* plain, int paddedLen, unsigned char* padded);

#endif