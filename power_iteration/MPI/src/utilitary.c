#include <stdio.h>
#include <math.h>
#include <string.h>

double max (double a, double b) {
	if (a > b) return a;
	return b;
}

double fabs(double a) {
	if (a < 0.0) a = -a;
	return a;
}

void printVec(double *vec, int N) {
	for (int i = 0; i < N; i++) {
		printf("%f\n", vec[i]);
	}
}

double compareVec(double *vec1, double *vec2, int N) {
	double sum = 0.0;
	for (int i = 0; i < N; i++) {
		sum += fabs(vec1[i] - vec2[i]);
	}
	
	return sum;
}

double dotProd(double *x, double *y, int N) {
	double sum = 0.0;
	for (int i = 0; i < N; i++) {
		sum += x[i] * y[i];
	}
	return sum;
}


double vecNorm(double *vec, int N) {
	/*double sum = 0.0;
	for (int i = 0; i < N; i++) {
		sum += vec[i] * vec[i];
	}
	return sqrt(sum);*/

	return sqrt(dotProd(vec, vec, N));
}

void residual(double *res, int *row, int *col, double *nnz, double *b, double *x, int N) {
	for (int i = 0; i < N; i++) {
		res[i] = b[i];
		for (int j = row[i]; j < row[i+1]; j++) {
			res[i] -= nnz[j] * x[col[j]];
		}
	}
}


double residualNorm(double *res, int *row, int *col, double *nnz, double *b, double *x, int N) {
	residual(res, row, col, nnz, b, x, N);
	return vecNorm(res, N);
}

void dgemvCSR(int *row, int *col, double *nnz, double *x, double *result, int N) {

	memset(result, 0.0, sizeof(double) * N);
	for (int i = 0; i < N; i++) {
		for (int j = row[i]; j < row[i+1]; j++) {
			result[i] += nnz[j] * x[col[j]];
		}
	}
	
}
