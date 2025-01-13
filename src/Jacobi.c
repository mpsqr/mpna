#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utilitary.h"

#define MAX_ITER 1000
#define TOL 1e-6


void JacobiCSR(int *row, int *col, double *nnz, double *b, double *x, int N) {
	
	printf("Starting the Jacobi solver...\n");

	double *newX = (double *)malloc(sizeof(double) * N);
	double *diag = (double *)malloc(sizeof(double) * N);

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

			newX[i] = (b[i] - sum) * 0.25;
		}

		double maxError = 0.0;
		for (int i = 0; i < N; i++) {
			maxError = max(maxError, fabs(newX[i] - x[i]));
			x[i] = newX[i];
		}

		//printf("%f\n", maxError);
		// Check for convergence
		if (maxError < TOL) {
			printf("Jacobi converged in %d iterations.\n", iter+1);
			break;
		}
	}

		

	free(newX);
	free(diag);

}
