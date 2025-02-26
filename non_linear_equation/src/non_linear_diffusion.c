#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "HYPRE.h"
#include "HYPRE_IJ_mv.h"
#include "HYPRE_parcsr_ls.h"

// Parameters
#define N 100                // Number of discretization points
#define dx (1.0 / N)         // Mesh size
#define dt 0.01              // Time step
#define kappa0 0.01          // Diffusion coefficient constant
#define sigma 0.1            // Radiation constant
#define beta 1.0             // Source term constant
#define gamma 0.1            // Stability parameter
#define TOL 1e-6             // Convergence tolerance
#define MAX_ITER 1000        // Maximum iterations

// Function to compute kappa(u)
double kappa(double u) {
    return kappa0 * u;       // Example: kappa(u) = kappa0 * u
}

// Function to compute the source term Q(x)
double Q(double x) {
    return (x <= 0.2) ? beta : 0.0; // Source term in the flame region
}

// Main function
int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);	

    // Initialize variables
    double u[N+1], u_new[N+1]; // Temperature arrays
    double rhs[N+1];           // Right-hand side vector
    double umax = 1.0;         // Initial guess for maximum temperature
    int iter = 0;              // Iteration counter
    double error = 1.0;        // Error for convergence check

    // Initialize temperature array
    for (int i = 0; i <= N; i++) {
        u[i] = 1.0;            // Initial guess for temperature
    }

    // Time-stepping loop
    while (iter < MAX_ITER && error > TOL) {
        // Create Hypre matrix and vectors
        HYPRE_IJMatrix A;
        HYPRE_ParCSRMatrix parcsr_A;
        HYPRE_IJVector b, x;
        HYPRE_ParVector par_b, par_x;

	HYPRE_Int status;
        HYPRE_BigInt ilower = rank * (N + 1) / size;
	HYPRE_BigInt iupper = (rank + 1) * (N + 1) / size - 1;
        HYPRE_IJMatrixCreate(MPI_COMM_WORLD, ilower, iupper, ilower, iupper, &A);
	HYPRE_IJMatrixSetObjectType(A, HYPRE_PARCSR);
        status = HYPRE_IJMatrixInitialize(A); // THIS FAILS

        printf("status: %d\n", status);
        HYPRE_IJVectorCreate(MPI_COMM_WORLD, 0, N, &b);
        HYPRE_IJVectorSetObjectType(b, HYPRE_PARCSR);
        HYPRE_IJVectorInitialize(b);

        HYPRE_IJVectorCreate(MPI_COMM_WORLD, 0, N, &x);
        HYPRE_IJVectorSetObjectType(x, HYPRE_PARCSR);
        HYPRE_IJVectorInitialize(x);

        // Assemble matrix A and right-hand side vector b
        for (int i = 0; i <= N; i++) {
	    double x_pos = i * dx;	    

	    double kappa_plus = (i < N) ? 0.5 * (kappa(u[i+1]) + kappa(u[i])) : 0.0;
	    double kappa_minus = (i > 0) ? 0.5 * (kappa(u[i-1]) + kappa(u[i])) : 0.0;
            // Diagonal entry
            double a_i = 1.0 + dt * ((kappa_plus + kappa_minus) / (dx * dx) + 4.0 * sigma * pow(u[i], 3));
            HYPRE_BigInt rows[1] = {i};
            HYPRE_Int ncols[1] = {1};
	    HYPRE_BigInt cols[1] = {i};
	    HYPRE_Real values[1] = {a_i};
            HYPRE_IJMatrixSetValues(A, 1, rows, ncols, cols, values);

            // Off-diagonal entries
            if (i > 0) {
                double c_i = -dt * kappa_minus / (dx * dx);
                HYPRE_BigInt cols_minus[1] = {i - 1};
		HYPRE_Real values_minus[1] = {c_i};
                HYPRE_IJMatrixSetValues(A, 1, rows, ncols, cols_minus, values_minus);
            }
            if (i < N) {
                double b_i = -dt * kappa_plus / (dx * dx);
                HYPRE_BigInt cols_plus[1] = {i + 1};
		HYPRE_Real values_plus[1] = {b_i};
                HYPRE_IJMatrixSetValues(A, 1,rows, ncols, cols_plus, values_plus);
            }

            // Right-hand side vector
            rhs[i] = u[i] + dt * (Q(x_pos) + sigma);
        }


        // Set boundary conditions
        rhs[0] = u[0]; // Neumann condition (mirror)
        rhs[N] = 1.0;  // Dirichlet condition


        // Finalize matrix and vectors
	HYPRE_IJMatrixAssemble(A);
        HYPRE_IJMatrixGetObject(A, (void**) &parcsr_A);

        HYPRE_IJVectorAssemble(b);
        HYPRE_IJVectorGetObject(b, (void**) &par_b);

        HYPRE_IJVectorAssemble(x);
	HYPRE_IJVectorGetObject(x, (void **) &par_x);
       
	 // Solve the linear system using Hypre's BoomerAMG solver
        HYPRE_Solver solver;
        HYPRE_BoomerAMGCreate(&solver);
        HYPRE_BoomerAMGSetTol(solver, TOL);
        HYPRE_BoomerAMGSetMaxIter(solver, 100);
        HYPRE_BoomerAMGSetup(solver, parcsr_A, par_b, par_x);
        HYPRE_BoomerAMGSolve(solver, parcsr_A, par_b, par_x);


        // Extract solution
        double *solution = (double*) malloc((N+1) * sizeof(double));
        HYPRE_IJVectorGetValues(x, N+1, NULL, solution);

        // Update temperature array and compute error
        error = 0.0;
        for (int i = 0; i <= N; i++) {
            u_new[i] = solution[i];
            error += fabs(u_new[i] - u[i]);
        }
        error /= (N+1);

        // Update u for the next iteration
        for (int i = 0; i <= N; i++) {
            u[i] = u_new[i];
        }

        // Clean up Hypre objects
        HYPRE_BoomerAMGDestroy(solver);
        HYPRE_IJMatrixDestroy(A);
        HYPRE_IJVectorDestroy(b);
        HYPRE_IJVectorDestroy(x);

        free(solution);
        iter++;
    }

    // Output the final temperature distribution
    printf("Final temperature distribution:\n");
    for (int i = 0; i <= N; i++) {
        printf("u[%d] = %f\n", i, u[i]);
    }

    MPI_Finalize();

    return 0;
}
