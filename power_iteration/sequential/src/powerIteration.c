#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "utilitary.h"

#define MAX_ITER 100000
#define TOL 1e-18

void powerIteration(int *row, int *col, double *nnz, double *b, int N) {

	double *bkp1 = (double *)malloc(sizeof(double) * N);
	double invNorm = 0.0;

	float startTime = (float)clock()/CLOCKS_PER_SEC;

	for (int i = 0; i < MAX_ITER; i++) {
		double conv = 0.0; // For convergence

		dgemvCSR(row, col, nnz, b, bkp1, N);



		invNorm = 1.0 / vecNorm(bkp1, N);


		for (int j = 0; j < N; j++) {
			double temp = b[j];

			b[j] = bkp1[j] * invNorm;

			conv += (temp - b[j]) * (temp - b[j]);
		}
		
		if (sqrt(conv) < TOL) {
			float endTime = (float)clock()/CLOCKS_PER_SEC;
			printf("Power iteration converged after %d iterations (%fs).\n", i+1, endTime-startTime);
			break;
		}
	}

	free(bkp1);
}
