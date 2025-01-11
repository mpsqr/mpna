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


	int totalElem = numOfElements(N); // Non zero elements

	// Allocating resources for the CSR matrix
	int *row = (int *)malloc(sizeof(int) * ((N*N) + 1));
	int *col = (int *)malloc(sizeof(int) * totalElem);
	int *nnz = (int *)malloc(sizeof(int) * totalElem);

	// Fill the sparse matrix with the kernel
	fillSparseMatrix(row, col, nnz, N);
	
		
	
	free(row);
	free(col);
	free(nnz);	

	return EXIT_SUCCESS;
}
