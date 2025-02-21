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

    int nCols, nRows, nValues, nColsLocal, nRowsLocal, nValuesLocal, step;
    fscanf(file, "%d %d %d", &nCols, &nRows, &nValues);

    step = nRows / size;
    nRowsLocal = step;
    if (rank == size-1) {
      nRowsLocal += nRows % size;
    }

    row = (int *)malloc(sizeof(int) * (nRowsLocal + 1));
    col = (int *)malloc(sizeof(int) * nValues);
    nnz = (double *)malloc(sizeof(double) * nValues);

    fclose(file);

	b = (double *)malloc(sizeof(double) * nRowsLocal);
	fillVectorFile("./data/largeVector.vec", b, step * rank, nRowsLocal);


    free(row);
    free(col);
    free(nnz);
    free(b);

    MPI_Finalize();
	return EXIT_SUCCESS;
}
