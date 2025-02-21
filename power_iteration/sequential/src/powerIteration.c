#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utilitary.h"

#define MAX_ITER 100000
#define TOL 1e-18

void powerIteration(int *row, int *col, double *nnz, double *b, int N) {

	double *bkp1 = (double *)malloc(sizeof(double) * N);
	double *prev = (double *)malloc(sizeof(double) * N);
	double invNorm = 0.0;

	for (int i = 0; i < MAX_ITER; i++) {
		memcpy(prev, b, sizeof(double) * N); // For checking convergence

		dgemvCSR(row, col, nnz, b, bkp1, N);



		invNorm = 1.0 / vecNorm(bkp1, N);


		for (int j = 0; j < N; j++) {
			b[j] = bkp1[j] * invNorm;
			prev[j] -= b[j];
		}


		if (vecNorm(prev, N) < TOL) {
			printf("Power Iteration converged after %d iterations.\n", i + 1);
			break;
		}
	}

	free(bkp1);
	free(prev);
}