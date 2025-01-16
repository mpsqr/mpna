#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utilitary.h"
#include <math.h>

#define MAX_ITER 10000
#define TOL 1e-6


void JacobiCSR(int *row, int *col, double *nnz, double *b, double *x, int N) {
	
	printf("Starting the Jacobi solver...\n");

	double *newX = (double *)malloc(sizeof(double) * N);
	double *diag = (double *)malloc(sizeof(double) * N);
	double *res = (double *)malloc(sizeof(double) * N);

	memset(newX, 0.0, N);
	
	// Get the diagonal
	for (int i = 0; i < N; i++) {
		for (int j = row[i]; j < row[i+1]; j++) {
			if (col[j] == i) {
				diag[i] = nnz[j];
			}
		}
	}

	// Iteration
	for (int iter = 0; iter < MAX_ITER; iter++) {
		for (int i = 0; i < N; i++) {
			double sum = 0.0;
			for (int j = row[i]; j < row[i+1]; j++) {
				if (col[j] != i) {sum += nnz[j] * x[col[j]];}
			}

			newX[i] = (b[i] - sum) * 0.25; // Divide by diagonal
		}

		// Check for convergence
		if (residualNorm(res, row, col, nnz, b, newX, N) < TOL) {
			printf("Jacobi converged in %d iterations.\n", iter + 1);
			break;
		}
		
		memcpy(x, newX, sizeof(double) * N);
	}

		

	free(newX);
	free(diag);
	free(res);
}
