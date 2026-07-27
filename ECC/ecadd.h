#include "bigIntPlusMinus.h"
#include "bigIntMulSqrRed.h"
#include "bigIntInv.h"
#include "bigIntGlobal.h"
#include "eccGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ECADDAfAf(unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int x2[], unsigned int y2[], int is_inf2, unsigned int x3[], unsigned int y3[], int* is_inf3);

void ECADDJaAf(unsigned int X1[], unsigned int Y1[], unsigned int Z1[], unsigned int x2[], unsigned int y2[], int is_inf, unsigned int X3[], unsigned int Y3[], unsigned int Z3[]);

void ECADDJaJa(unsigned int X1[], unsigned int Y1[], unsigned int Z1[], unsigned int X2[], unsigned int Y2[], unsigned int Z2[], unsigned int X3[], unsigned int Y3[], unsigned int Z3[]);