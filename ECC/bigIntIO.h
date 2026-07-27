#ifndef BIGIO_H
#define BIGIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bigIntInput(unsigned int data[], FILE* fp, int len);
void bigIntOutput(unsigned int data[], FILE* fp, int len);

#endif