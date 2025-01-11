
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


