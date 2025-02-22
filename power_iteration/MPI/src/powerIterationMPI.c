#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utilitary.h"
#include <mpi.h>

#define MAX_ITER 10000
#define TOL 1e-12

void powerIterationMPI(int *row, int *col, double *nnz, double *b, double *res, int N, int Nlocal, int rank, int size) {

	double invNorm = 0.0;

	int step = N / size;

	MPI_Request req;
	MPI_Status sta;

	double start = MPI_Wtime();

	for (int i = 0; i < MAX_ITER; i++) {
		double conv = 0.0; // Convergence

		dgemvCSR(row, col, nnz, b, &res[rank * step], Nlocal);

		for (int p = 0; p < size; p++) {
			if (p != rank) {
				MPI_Isend(&res[rank * step], Nlocal, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, &req);
			}
		}
		
		for (int p = 0; p < size; p++) {
			if (p != rank) {
				int count = step;
			    if (p == size-1) {
			      count += N % size;
			    }
				MPI_Recv(&res[p*step], count, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, &sta);
			}
		}


		invNorm = 1.0 / vecNorm(res, N);

		for (int j = 0; j < N; j++) {
			double temp = b[j];
			b[j] = res[j] * invNorm;
			conv += (temp - b[j]) * (temp - b[j]);
		}

		if (sqrt(conv) < TOL) {
			double end = MPI_Wtime();
			printf("RANK %d: Power Iteration converged after %d iterations (%lf s).\n", rank, i + 1, end-start);
			break;
		}
	}
}