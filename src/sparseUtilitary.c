#include <stdio.h>
#include <stdlib.h>
// Computes the total number of elements in the matrix
int numOfElements(int N) {
	//if (N == 1) return 1; // Only one element

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



void fillSparseMatrix(int *row, int *col, double *nnz, int N) {
	
	printf("Filling the sparse matrix with the kernel...\n");

	row[0] = 0;
	int rowInd = 0;
	int totalElem = 0;
	for (int iter = 0; iter < N*N; iter++) {
		int i = iter / N;
		int j = iter % N;

		if (i > 0) { // Top point
			nnz[totalElem] = -1.0;
			col[totalElem] = iter-N;
			totalElem++;
		}
		if (j > 0) { // Left point
			nnz[totalElem] = -1.0;
			col[totalElem] = iter-1;
			totalElem++; 
		}
		

		// Self point
		nnz[totalElem] = 4.0;
		col[totalElem] = iter;
		totalElem++;
		
		if (j < N-1) { // Right point
			nnz[totalElem] = -1.0;
			col[totalElem] = iter+1;
			totalElem++;
		}
		
		if (i < N-1) { // Bottom point
			nnz[totalElem] = -1.0;
			col[totalElem] = iter+N;
			totalElem++;
		}
			
		rowInd += 1;
		row[rowInd] = totalElem;
	}

	printf("Sparse matrix filled.\n");

}

int fill_matrix_file(char *strpath, int *row, int *col, double *nnz) {

    FILE *f = fopen(strpath, "r");
    if (!f)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    

    int dim_x = 0;
    int dim_y = 0;
    int NNZ = 0;
    fscanf(f, "%d %d %d\n", &dim_x, &dim_y, &NNZ);

    //printf("%d %d %d\n", dim_x, dim_y, NNZ);

    nnz = (double *)malloc(sizeof(double) * NNZ);
    row = (int *)malloc(sizeof(int) * dim_x);
    col = (int *)malloc(sizeof(int) * dim_y);

    int irow = 0;
    int icol = 0;

    int old_row = 0;
    int row_index = 0;
    double value = 0;

    for (int i = 0; i < NNZ; ++i)
    {
        fscanf(f, "%d %d %lf\n", &icol, &irow, &value);

        irow = irow - 1;
        icol = icol - 1;
        printf("%d %d %lf\n", icol, irow, value);

        if (irow != old_row)
        {
            row_index++;
            row[row_index] = i;
            old_row = irow;
        }
        col[i] = icol;
        nnz[i] = value;
    }

    fclose(f);
}
