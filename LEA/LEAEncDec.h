#ifndef LEAENCDEC_H
#define LEAENCDEC_H

void keySchedule(int keyLen, unsigned char* key, unsigned int encKey[][6], unsigned int decKey[][6]);
void encRound(unsigned int plain[4], unsigned int encKey[]);
void decRound(unsigned int cipher[4], unsigned int decKey[]);
void encrypt(unsigned char plain[], int keyLen, unsigned int encKey[][6], unsigned char cipher[]);
void decrypt(unsigned char cipher[], int keyLen, unsigned int decKey[][6], unsigned char plain[]);

#endif