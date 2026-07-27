#ifndef LEAGLOBAL_H
#define LEAGLOBAL_H

#define MAXBLOCK 1000
#define ROL(x, n) (((x) << (n)) | ((x) >> 32 - (n)))
#define ROR(x, n) (((x) >> (n)) | ((x) << 32 - (n)))

extern unsigned int delta[8];

#endif