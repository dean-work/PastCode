/* Skeleton code for Gauss-Seidel/Jacobi method
   Must be slightly modified for Point Jacobi method
   to sovle system of linear equation [A]{X} = {C}
   accepting input data from in_gs_jacobi.csv
*/


#include<stdio.h>
#include<stdlib.h>
#include<math.h>

// functions for iterative methods
void GS_solve();

void Jacobi_solve();

int main(int argc, char *argv[]) {
    
	// Parse the input file here
	
	// building A, X, C for the equation [A]{X} = {C}
	//  - first, creating empty matrix & vectors 	
	
	// open data file for reading coefficients
	
	//  - then, read [A] and {C} from input file
	
    // print the original data to see it was read in 
    // correctly
	
	// solve the equation
	GS_solve();
	
	// solve the equation
	Jacobi_solve();

	// print to solution

    return 0;
}


// solving, using Gauss-Seidel
void GS_solve() {

	// creating interim {R}

    // read initial guesses
 
 	/* Compute the intial residual */

    /* write intitial residual */

    // Iterative Loop using EPS

}

// solving, using Jacobi
void Jacobi_solve() {

}