#ifndef COMPLEXMATRIXINVERSION
#define COMPLEXMATRIXINVERSION

#include <iostream>
#include <complex>
using namespace std;

float** Add(float** a, float** b, int n, int m) ;
float** Mul(float** a, float** b, int n, int m, int o) ;
float** inv(float** a, int num);
void swap(float* a, float* b); //declare subroutine

/**
Calculate inverse matrix
@param src Invertible matrix
@param n Matrix dimension
*/
complex<float>** GetMatrixInverse(complex<float>** src, int n) ;


#endif