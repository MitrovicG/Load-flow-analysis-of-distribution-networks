#include "ComplexMatrixInversion.h"

#include <iostream>
#include<complex>
using namespace std;


 // Matrix addition a+b, where a and b are both n*m matrix
float** Add(float** a, float** b, int n, int m) {
	float** res = new float* [n];
	for (int i = 0; i < n; i++) res[i] = new float[m];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			res[i][j] = a[i][j] + b[i][j];
		}
	}
	return res;
}

// Matrix multiplication, a*b, a is n*m matrix, b is m*o matrix
float** Mul(float** a, float** b, int n, int m, int o) {
	float** res = new float* [n];
	float temp = 0.0;
	for (int i = 0; i < n; i++) res[i] = new float[o];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < o; j++) {
			temp = 0.0;
			for (int k = 0; k < m; k++) {
				temp += a[i][k] * b[k][j];
			}
			res[i][j] = temp;
		}
	}
	return res;
}

// Find the inverse of the real matrix, return the inverse of a, where a is a square matrix of num type
float** inv(float** a, int num)
{
	int* is, * js, i, j, k;
	int n = num;
	float temp, fmax;
	float** tp = new float* [num];
	for (int i = 0; i < num; i++) tp[i] = new float[num];
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			tp[i][j] = a[i][j];
		}
	}
	is = new int[n];
	js = new int[n];
	for (k = 0; k < n; k++)
	{
		fmax = 0.0;
		for (i = k; i < n; i++) {
			for (j = k; j < n; j++)
			{
				 temp = fabs(tp[i][j]);//Find the maximum
				if (temp > fmax)
				{
					fmax = temp;
					is[k] = i; js[k] = j;
				}
			}
		}

		if ((fmax + 1.0) == 1.0)
		{
			delete[] is;
			delete[] js;
			return NULL;
		}
		if ((i = is[k]) != k)
			for (j = 0; j < n; j++)
				 swap(&tp[k][j], &tp[i][j]);//Exchange pointer
		if ((j = js[k]) != k)
			for (i = 0; i < n; i++)
				 swap(&tp[i][k], &tp[i][j]); //Exchange pointer
		tp[k][k] = 1.0 / tp[k][k];
		for (j = 0; j < n; j++)
			if (j != k)
				tp[k][j] *= tp[k][k];
		for (i = 0; i < n; i++)
			if (i != k)
				for (j = 0; j < n; j++)
					if (j != k)
						tp[i][j] = tp[i][j] - tp[i][k] * tp[k][j];
		for (i = 0; i < n; i++)
			if (i != k)
				tp[i][k] *= -tp[k][k];
	}
	for (k = n - 1; k >= 0; k--)
	{
		if ((j = js[k]) != k)
			for (i = 0; i < n; i++)
				swap(&tp[j][i], &tp[k][i]);
		if ((i = is[k]) != k)
			for (j = 0; j < n; j++)
				swap(&tp[j][i], &tp[j][k]);
	}
	delete[] is;
	delete[] js;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
		}
	}
	return tp;
}
void swap(float* a, float* b)
{
	float c;
	c = *a;
	*a = *b;
	*b = c;
}

//Calculate inverse matrix
complex<float>** GetMatrixInverse(complex<float>** src, int n) {
	float** A = new float* [n];
	float** B = new float* [n];
	for (int i = 0; i < n; i++) {
		A[i] = new float[n];
		B[i] = new float[n];
		for (int j = 0; j < n; j++) {
			A[i][j] = src[i][j].real();
			B[i][j] = src[i][j].imag();
		}
	}
	float** A1 = inv(A, n);
	float** A1B = Mul(A1, B, n, n, n);
	float** BA1B = Mul(B, A1B, n, n, n);
	float** AjBA1B = Add(A, BA1B, n, n);
	float** AjBA1B_1 = inv(AjBA1B, n);
	float** A1B_AjBA1B_1 = Mul(A1B, AjBA1B_1, n, n, n);

	complex<float>** res = new complex<float> * [n];
	for (int i = 0; i < n; i++) {
		res[i] = new complex<float>[n];
		for (int j = 0; j < n; j++) {
			res[i][j].real(AjBA1B_1[i][j]);
			res[i][j].imag(-1.0 * A1B_AjBA1B_1[i][j]);
		}
	}
	return res;
}