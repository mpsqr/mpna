#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilitary.h"
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

	int *row = (int *)malloc(sizeof(int) * (N + 1));	

	int totalElements = numOfElements(N);
	printf("%d\n", totalElements);

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			
		}
	}



	return EXIT_SUCCESS;
}
