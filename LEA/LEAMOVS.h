#ifndef LEAMOVS_H
#define LEAMOVS_H

void LEA_ALL_ECB_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher);
void LEA_ALL_CBC_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher);
void LEA_ALL_CTR_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher);
void LEA_ALL_CFB1_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int s, int* plainLenBit, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLenBit, unsigned char* cipher);
void LEA_ALL_CFB8_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int s, int* plainLenBit, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLenBit, unsigned char* cipher);
void LEA_ALL_CFB64_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int s, int* plainLenBit, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLenBit, unsigned char* cipher);
void LEA_ALL_CFB128_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int s, int* plainLenBit, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLenBit, unsigned char* cipher);
void LEA_ALL_OFB_KAT_MMT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher);
void LEA_ALL_ECB_MCT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher);
void LEA_ALL_CBC_MCT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher);
void LEA_ALL_CTR_MCT(int* keyLen, unsigned char* key, unsigned char* IV, int* plainLen, unsigned char* plain, unsigned int encKey[][6], unsigned int decKey[][6], int* cipherLen, unsigned char* cipher);

#endif