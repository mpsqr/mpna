#include <stdlib.h>
#include <string.h>

#define MAX_ITER 1000

int max(int a, int b) {
	if (a > b) return a;
	return b;
}



void JacobiCSR(int *row, int *col, int *nnz, int *b, int *x, int N) {
	int *newX = (int *)malloc(sizeof(int) * N);
	int *diag = (int *)malloc(sizeof(int) * N);

	memset(newX, 0, N);
	
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
			int sum = 0;
			for (int j = row[i]; j < row[i+1]; j++) {
				if (col[j] != i) {sum += nnz[j] * x[col[j]];}
			}

			newX[i] = (b[i] - sum) / diag[i];
		}

		int maxDiff = 0;
		for (int i = 0; i < N; i++) {
			maxDiff = max(maxDiff, abs(newX[i] - x[i]));
			x[i] = newX[i];
		}
	}


	free(newX);
	free(diag);

}
