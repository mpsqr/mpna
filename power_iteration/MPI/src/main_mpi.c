#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#include "utilitary.h"
#include "sparseUtilitary.h"
#include "powerIterationMPI.h"

#define MAX 100

/*
	make -f Makefile.mpi
	mpiexec -n <proc> ./bin/main_mpi
*/


int main(int argc, char **argv) {

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *row, *col;
	double *nnz, *b;


    FILE *file = fopen("./data/bcsstk03.mtx", "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    int nCols, nRows, nValues, nRowsLocal, step;

    fscanf(file, "%d %d %d", &nCols, &nRows, &nValues);

    step = nRows / size;
    nRowsLocal = step;
    if (rank == size-1) {
      nRowsLocal += nRows % size;
    }


    row = (int *)malloc(sizeof(int) * (nRowsLocal + 1));
    col = (int *)malloc(sizeof(int) * nValues);
    nnz = (double *)malloc(sizeof(double) * nValues);

    int rowVal = 0;
    int colVal = 0;
    double nnzVal;

    row[0] = 0;
    int lastRow = 0;
    int rowSum = 0;
    int rowInd = 0;

    fscanf(file, "%d %d %lf", &colVal, &rowVal, &nnzVal);
    while(rowVal < step * rank) { // Skip the lines
    	fscanf(file, "%d %d %lf", &colVal, &rowVal, &nnzVal);
    	rowVal--;
    }


   	while ((rowVal-1) < ((step * rank) + nRowsLocal)) { // Construct the local CSR matrix

   		rowVal -= step * rank; // Locally, all row vectors are 0 indexed
   		rowVal--; // Matrix is 1-indexed
        colVal--;

        // Fill in row pointers for missing rows
        while (rowInd < rowVal) {
            rowInd++;
            row[rowInd] = rowSum; // Mark start of new row
        }
        
        nnz[rowSum] = nnzVal;
        col[rowSum] = colVal;
        rowSum++; // Track total nonzero elements seen so far


        fscanf(file, "%d %d %lf", &colVal, &rowVal, &nnzVal);
        if (feof(file)) {
        	break;
        }
   	}
   	row[rowInd + 1] = rowSum;



   	// Reallocate to correct sizes
   	nnz = (double *)realloc(nnz, sizeof(double) * rowSum);
   	col = (int *)realloc(col, sizeof(int) * rowSum);

    fclose(file);


	b = (double *)malloc(sizeof(double) * nCols);
	fillVectorFile("./data/largeVector.vec", b, 0, nCols);
	double *res = (double *)malloc(sizeof(double) * nRows);


	powerIterationMPI(row, col, nnz, b, res, nRows, nRowsLocal, rank, size);

    free(row);
    free(col);
    free(nnz);
    free(b);
    free(res);

    MPI_Finalize();
	return EXIT_SUCCESS;
}
