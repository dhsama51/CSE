#ifndef BIGMULSQRRED_H
#define BIGMULSQRRED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mulSplit(unsigned int a, unsigned int b, unsigned int* U, unsigned int* V);
void mulExpand(unsigned int a, unsigned int b, unsigned int* U, unsigned int* V);
void bigIntMulOS(unsigned int a[], unsigned int b[], unsigned int c[]);
void bigIntMulPS(unsigned int a[], unsigned int b[], unsigned int c[]);
void bigIntMulKaratsuba(unsigned int a[], unsigned int b[], int size, unsigned int c[]);
void bigIntSquare(unsigned int a[], unsigned int c[], int len);
void FpReduct(unsigned int a[], unsigned c[], int len);
void FpReduct(unsigned int a[], unsigned c[], int len);

#endif