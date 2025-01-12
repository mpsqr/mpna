#include <stdio.h>


double max (double a, double b) {
	if (a > b) return a;
	return b;
}

double fabs(double a) {
	if (a < 0.0) return -a;
	return a;
}

void printVec(double *vec, int N) {
	for (int i = 0; i < N; i++) {
		printf("%f\n", vec[i]);
	}
}
