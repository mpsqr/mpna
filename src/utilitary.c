#include <stdio.h>


double max (double a, double b) {
	if (a > b) return a;
	return b;
}

double fabs(double a) {
	if (a < 0.0) a = -a;
	return a;
}

void printVec(double *vec, int N) {
	for (int i = 0; i < N; i++) {
		printf("%f\n", vec[i]);
	}
}

double compareVec(double *vec1, double *vec2, int N) {
	double sum = 0.0;
	for (int i = 0; i < N; i++) {
		sum += fabs(vec1[i] - vec2[i]);
	}
	
	return sum;
}
