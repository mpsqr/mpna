
double max(double, double);
double fabs(double);

void printVec(double *, int);
double compareVec(double *, double *, int);

double dotProd(double *, double *, int);
double vecNorm(double *, int);
void residual(double *, int *, int *, double *, double *, double *, int);
double residualNorm(double *, int *, int *, double *, double *, double *, int);
void dgemvCSR(int *, int *, double *, double *, double *, int);
