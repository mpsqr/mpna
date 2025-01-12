#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilitary.h"

#define MAX_ITER 1000
#define TOL 1e-6


void gaussSeidelCSR(int *row, int *col, double *nnz, double *b, double *x, int N) {

	printf("Starting the Gauss-Seidel solver...\n");
	
	double *oldX = (double *)malloc(sizeof(double) * N);
	

	for (int iter = 0; iter < MAX_ITER; iter++) {
		memcpy(oldX, x, N);
		
		// Computing the GS method
		for (int i = 0; i < N; i++) {
			double sum = 0.0;
			double temp = 0.0;

			for (int j = row[i]; j < row[i+1]; j++) {
				int colInd = col[j];
				if (colInd == i) {
					temp = nnz[j];
				} else {
					sum += nnz[j] * x[colInd];
				}
			}
		
			x[i] = (b[i] - sum) / temp;
		}
		// Check the  difference
		double maxError = 0.0;
		for (int i = 0; i < N; i++) {
			maxError = max(maxError, fabs(x[i] - oldX[i]));
		}
	

		if (maxError < TOL) {
			printf("Gauss-Seidel converged in %d iterations.\n", iter+1);
			break;
		}
	}

	free(oldX);
}
