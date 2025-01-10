#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	int size = N * N;

	printf("Total size: %d\n", size);





	return EXIT_SUCCESS;
}
