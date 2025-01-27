#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utilitary.h"

#define MAX_ITER 10
#define TOL 1e-12


void CSR_GMRES(int *row, int *col, double *nnz, double *b, double *x, int N) {

	printf("Performing GMRES...\n");
	
	// Allocation
	double *Q = (double *)malloc(sizeof(double) * (N * (MAX_ITER + 1)));
	double *H = (double *)malloc(sizeof(double) * (MAX_ITER * (MAX_ITER + 1)));
	memset(H, 0.0, sizeof(double) * (MAX_ITER * (MAX_ITER + 1)));

	double *r = (double *)malloc(sizeof(double) * N); // r0
	double *norms = (double *)malloc(sizeof(double) * (MAX_ITER + 1));
	double *y = (double *)malloc(sizeof(double) * MAX_ITER); // Least-squares solution


	// Norm 1 initial vector
	memset(norms, 0.0, sizeof(double) * (MAX_ITER + 1));
	norms[0] = residualNorm(r, row, col, nnz, b, x, N);


	double inv = 1.0 / norms[0];
	for (int i = 0; i < N; i++) {
		Q[i] = r[i] * inv;
	}		
	// Arnoldi iterations
	for (int iter = 0; iter < MAX_ITER; iter++) {
		// Compute Qk using Qk-1
		int Qk = (iter + 1) * N;
		dgemvCSR(row, col, nnz, (Q + (Qk-N)), (Q + Qk), N);
		
		for (int j = 0; j <= iter; j++) {
			int Hjkm1 = (j * MAX_ITER) + iter; // Hj,k-1
			double prod = dotProd((Q + (j*N)), (Q + Qk), N); // Hj,k-1 = Qj•Qk
			H[Hjkm1] = prod;
			
			for (int k = 0; k < N; k++) { // Qk <- Qk - Hj,k-1*Qj
				Q[Qk + k] -= prod * Q[(j*N) + k];
			}
			// printf("%f\n", dotProd(Q+Qk, Q+(j*N), N)); // Check for orthonormality (should be 0)
		}

		double norm = vecNorm(Q + Qk, N);
		H[((iter+1) * MAX_ITER) + iter] = norm; // Hk,k-1 <- ||Qk||
		norms[iter+1] = norm;

		double invNorm = 1.0 / norm;
		for (int k = 0; k < N; k++) {
			Q[Qk + k] *= invNorm; // Qk <- Qk/Hk,k-1
		}

		// Check lower triangular part
		printf("JKDHSIDFBQID\n");
		for (int i = 1; i < iter; i++) {
			for (int j = 0; j < i; j++) {
				int ind = i * iter + j;
				printf("%f\n", H[ind]);
			}
		}


		// NOT FINISHED DOESN'T WORK
		// Least-squares solution
		for (int i = iter; i >= 0; i--) {
			y[i] = norms[i];
			int Hi = i * MAX_ITER;
			for (int k = i + 1; k <= iter; k++) {
				y[i] -= H[Hi + k] * y[k];
			}
			y[i] /= H[Hi + i];
		}

		//printVec(y, MAX_ITER);

		// Compute new x
		memset(x, 0.0, sizeof(double) * N);
		for (int i = 0; i <= iter; i++) {
			for (int k = 0; k < N; k++) {
				x[k] += Q[(i * N) + k] * y[i];
			}
		}

		// Check for convergence
		double resNorm = residualNorm(r, row, col, nnz, b, x, N);
		//printf("%f\n", resNorm);
		if (resNorm < TOL) {
			printf("GMRES converged in %d iterations.", iter + 1);
			break;
		}
	}

	free(Q);
	free(H);
	free(r);
	free(norms);
	free(y);

}
