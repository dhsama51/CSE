#include "bigIntPlusMinus.h"
#include "bigIntMulSqrRed.h"
#include "bigIntInv.h"
#include "bigIntGlobal.h"
#include "ecadd.h"
#include "ecdbl.h"
#include "eccGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ECCSMLtRJa(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int X3[], unsigned int Y3[], unsigned int Z3[]);

void ECCSMLtRAf(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int x3[], unsigned int y3[], int* is_inf3);

void ECCSMRtLAf(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int x3[], unsigned int y3[], int* is_inf3);

int int2NAF(unsigned int k[], char kNAF[]);

void ECCSMLtRNAF(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int X3[], unsigned int Y3[], unsigned int Z3[]);

int int2wNAF(unsigned int k[], char kNAF[], int w);

void ECCSMLtRwNAF(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int X3[], unsigned int Y3[], unsigned int Z3[], int w);

void ECCSMSlideWin(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int X3[], unsigned int Y3[], unsigned int Z3[], int w);

void ECCSMFixedWin(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int X3[], unsigned int Y3[], unsigned int Z3[], int w);

void ECCSMFixedComb(unsigned int k[], unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int X3[], unsigned int Y3[], unsigned int Z3[], int w);

void ECCMSMShamir(unsigned int k[], unsigned int x1[], unsigned int y1[], unsigned int is_inf1, unsigned int l[], unsigned int x2[], unsigned int y2[], int is_inf2, unsigned int X3[], unsigned int Y3[], unsigned int Z3[], int w);

void ECCMSMInterleave(int v, unsigned int k_n[], unsigned int x_n[], unsigned int y_n[], int is_inf_n[], unsigned int X3[], unsigned int Y3[], unsigned int Z3[], int* w_n);