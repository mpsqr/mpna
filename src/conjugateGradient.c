#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utilitary.h"

#define MAX_ITER 10
#define TOL 1e-12

void conjugateGradientCSR(int *row, int *col, double *nnz, double *b, double *x, int N) {
	printf("Performing Conjugate Gradient...\n");

	double *r = (double *)malloc(sizeof(double) * N); //rk

	double *p = (double *)malloc(sizeof(double) * N); //p0

	double *Ap = (double *)malloc(sizeof(double) * N);

	residual(r, row, col, nnz, b, x, N); //r0 <- b - Ax
	memcpy(p, r, sizeof(double) * N);



	for (int k = 0; k < MAX_ITER; k++) {


		double rkDot = dotProd(r, r, N); // rk.rk
		dgemvCSR(row, col, nnz, p, Ap, N); // p.A
		double a_k = rkDot / dotProd(Ap, p, N); //αk = r.r / pAp
		
		for (int i = 0; i < N; i++) {
			x[i] += a_k * p[i];  //xk+1 <- xk + αkp
			r[i] -= a_k * Ap[i]; // rk+1 <- rk - αkAp
		}
		
		if (vecNorm(r, N) < TOL) {
			printf("Conjugate Gradient converged in %d iterations.\n", k);
			break;
		}

		double b_k = dotProd(r, r, N) / rkDot; // βk <- rk+1.rk+1 / rk.rk
		for (int i = 0; i < N; i++) {
			p[i] = r[i] + (b_k * p[i]); // pk+1 <- rk+1 + βk*pk
		}		

	}
	


	free(r);
	free(p);
	free(Ap);
}
