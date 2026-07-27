#include "bigIntPlusMinus.h"
#include "bigIntMulSqrRed.h"
#include "bigIntInv.h"
#include "bigIntGlobal.h"
#include "ecadd.h"
#include "eccGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ECDBLAf(unsigned int x1[], unsigned int y1[], int is_inf1, unsigned int x3[], unsigned int y3[], int* is_inf3);

void ECDBLJa(unsigned int X1[], unsigned int Y1[], unsigned int Z1[], unsigned int X3[], unsigned int Y3[], unsigned int Z3[]);