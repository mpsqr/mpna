#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sparseUtilitary.h"
	/*
 		Matéo Pasquier
		MPNA
		Usage: <N> matrix size
		Compilation gcc main.c -o main
	*/

#define MAX 10

int N;

int main(int argc, char **argv) {

	if (argc < 2) {
		printf("Usage: <N>\n");
		return EXIT_FAILURE;
	}	


	N = atoi(argv[1]);
	if (N < 1) {
		printf("N should be > 0.\n");
		return EXIT_FAILURE;
	}
	
	srand(0);

	int totalElem = numOfElements(N); // Non zero elements

	// Allocating resources for the CSR matrix
	int *row = (int *)malloc(sizeof(int) * ((N*N) + 1));
	int *col = (int *)malloc(sizeof(int) * totalElem);
	int *nnz = (int *)malloc(sizeof(int) * totalElem);

	// Fill the sparse matrix with the kernel
	fillSparseMatrix(row, col, nnz, N);
	
	int *b = (int *)malloc(sizeof(int) * (N*N));	
	for (int i = 0; i < N*N; i++) {
		b[i] = rand() % MAX;
	}

	int *x = (int *)malloc(sizeof(int) * (N*N));
	memset(x, 0, N*N);
	for (int i = 0; i < N*N; i++) printf("%d\n", x[i]);	


	free(row);
	free(col);
	free(nnz);
	free(b);
	free(x);

	return EXIT_SUCCESS;
}
