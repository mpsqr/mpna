#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utilitary.h"
#include "sparseUtilitary.h"
#include "Jacobi.h"
#include "GaussSeidel.h"

#define MAX 100
	/*
 		Matéo Pasquier
		MPNA
		Usage: <N> matrix size <seed> seed for the randomizer
		Compilation: make -> ./bin/main N seed
	*/


int N;

int main(int argc, char **argv) {

	if (argc < 2) {
		printf("Usage: <N> <seed>\n");
		return EXIT_FAILURE;
	}	


	N = atoi(argv[1]);
	if (N < 1) {
		printf("N should be > 0.\n");
		return EXIT_FAILURE;
	}

	if (argc > 2) {
		srand(atoi(argv[2]));
	}

	
	int totalElem = numOfElements(N); // Non zero elements

	// Allocating resources for the CSR matrix
	int *row = (int *)malloc(sizeof(int) * ((N*N) + 1));
	int *col = (int *)malloc(sizeof(int) * totalElem);
	double *nnz = (double *)malloc(sizeof(double) * totalElem);

	// Fill the sparse matrix with the kernel
	fillSparseMatrix(row, col, nnz, N);

	// Fill b with random values	
	double *b = (double *)malloc(sizeof(double) * (N*N));
	for (int i = 0; i < N*N; i++) {
		b[i] = (double)(rand() % MAX);
	}

	// x vector is initially set to 0
	double *xJacobi = (double *)malloc(sizeof(double) * (N*N));
	memset(xJacobi, 0.0, sizeof(double) * ( N*N));
	
	double *xGS = (double *)malloc(sizeof(double) * (N*N));
	memset(xGS, 0.0, sizeof(double) * ( N*N));


	// Solvers
	
	JacobiCSR(row, col, nnz, b, xJacobi, N*N);
	//printVec(xJacobi, N*N);
	

	gaussSeidelCSR(row, col, nnz, b, xGS, N*N);
	//printVec(xGS, N*N);


	printf("The total difference between Jacobi and GS is %f.\n", compareVec(xJacobi, xGS, N*N));	


	// Free the memory
	free(row);
	free(col);
	free(nnz);
	free(b);
	free(xJacobi);
	free(xGS);	

	return EXIT_SUCCESS;
}
