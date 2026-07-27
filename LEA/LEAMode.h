#ifndef LEAMODE_H
#define LEAMODE_H

void ECBenc(int plainLen, unsigned char* plain, int keyLen, unsigned int encKey[][6], int* cipherLen, unsigned char* cipher);
void ECBdec(int* plainLen, unsigned char* plain, int keyLen, unsigned int decKey[][6], int cipherLen, unsigned char* cipher);
void CBCenc(int plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][6], int* cipherLen, unsigned char* cipher);
void CBCdec(int* plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int decKey[][6], int cipherLen, unsigned char* cipher);
void leftShiftNbitIn16byte(unsigned char* input, int N);
void rightShiftNbitIn16byte(unsigned char* input, int N);
void CFBenc(int plainLenBit, unsigned char* plain, unsigned char* IV, int s, int keyLen, unsigned int encKey[][6], int* cipherLenBit, unsigned char* cipher);
void CFBdec(int* plainLenBit, unsigned char* plain, unsigned char* IV, int s, int keyLen, unsigned int encKey[][6], int cipherLenBit, unsigned char* cipher);
void OFBenc(int plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][6], int* cipherLen, unsigned char* cipher);
void OFBdec(int* plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][6], int cipherLen, unsigned char* cipher);
void CTRenc(int plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][6], int* cipherLen, unsigned char* cipher);
void CTRdec(int* plainLen, unsigned char* plain, unsigned char* IV, int keyLen, unsigned int encKey[][6], int cipherLen, unsigned char* cipher);

#endif