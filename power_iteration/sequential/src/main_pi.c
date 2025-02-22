#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utilitary.h"
#include "sparseUtilitary.h"
#include "powerIteration.h"

#define MAX 100


int main(int argc, char **argv) {

	int *row, *col;
	double *nnz, *b;


	FILE *file = fopen("./data/plbuckle.mtx", "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    int nCols, nRows, nValues;
    fscanf(file, "%d %d %d", &nCols, &nRows, &nValues);

    row = (int *)malloc(sizeof(int) * (nRows + 1));
    col = (int *)malloc(sizeof(int) * nValues);
    nnz = (double *)malloc(sizeof(double) * nValues);


    int rowVal, colVal;
    double nnzVal;

    row[0] = 0;
    int lastRow = 0;
    int rowSum = 0;
    int rowInd = 0;

    for (int i = 0; i < nValues; i++) {
        fscanf(file, "%d %d %lf", &colVal, &rowVal, &nnzVal);

        rowVal--; // Matrix is 1-indexed
        colVal--;

        // Fill in row pointers for missing rows
        while (rowInd < rowVal) {
            rowInd++;
            row[rowInd] = rowSum; // Mark start of new row
        }

        nnz[i] = nnzVal;
        col[i] = colVal;
        rowSum++; // Track total nonzero elements seen so far
    }
    row[rowInd + 1] = nValues;

	fclose(file);



	b = (double *)malloc(sizeof(double) * nRows);
	fillVectorFile("./data/largeVector.vec", b, 0, nRows);

	powerIteration(row, col, nnz, b, nRows);

	free(row);
	free(col);
	free(nnz);
	free(b);

	return EXIT_SUCCESS;
}
