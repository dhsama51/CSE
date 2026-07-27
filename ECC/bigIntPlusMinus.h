#ifndef BIGPM_H
#define BIGPM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bigIntPlus(unsigned int a[], unsigned int b[], int* carry, unsigned int c[], int len);
void bigIntMinus(unsigned int a[], unsigned int b[], int* borrow, unsigned int c[], int len);
int bigIntCmp(unsigned int data1[], unsigned int data2[], int len);
void FpPlus(unsigned int a[], unsigned int b[], int* carry, unsigned int c[], int len);
void FpMinus(unsigned int a[], unsigned int b[], int* borrow, unsigned int c[], int len);

#endif