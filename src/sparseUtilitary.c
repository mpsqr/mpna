#include <stdio.h>
// Computes the total number of elements in the matrix
int numOfElements(int N) {
	if (N == 1) return 1; // Only one element
	if (N == 2) return 12; // Only corners

	int sum = 0;
	sum += 12; // Corners
	sum += ((N-2)*(N-2)) * 5; // Center elements
	sum += ((N-2)*4) * 4; // Border lines
	return sum;
}
// Kernel
/*
  -1
-1 4 -1
  -1
*/



void fillSparseMatrix(int *row, int *col, int *nnz, int N) {
	
	printf("Filling the sparse matrix with the kernel...\n");

	row[0] = 0;
	int rowInd = 0;
	int totalElem = 0;
	for (int iter = 0; iter < N*N; iter++) {
		int i = iter / N;
		int j = iter % N;

		if (i > 0) { // Top point
			nnz[totalElem] = -1;
			col[totalElem] = iter-N;
			totalElem++;
		}
		if (j > 0) { // Left point
			nnz[totalElem] = -1;
			col[totalElem] = iter-1;
			totalElem++; 
		}
		

		// Self point
		nnz[totalElem] = 4;
		col[totalElem] = iter;
		totalElem++;
		
		if (j < N-1) { // Right point
			nnz[totalElem] = -1;
			col[totalElem] = iter+1;
			totalElem++;
		}
		
		if (i < N-1) { // Bottom point
			nnz[totalElem] = -1;
			col[totalElem] = iter+N;
			totalElem++;
		}
			
		rowInd += 1;
		row[rowInd] = totalElem;
	}

	printf("Sparse matrix filled.\n");

}
