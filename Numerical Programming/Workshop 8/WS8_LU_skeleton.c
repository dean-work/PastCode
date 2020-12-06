/* Skeleton code for LU decomposition method
   to sovle system of linear equation [A]{X} = {C}
   accepting input data from in_lu.csv
*/


#include<stdio.h>
#include<stdlib.h>

// functions for the LU-decomposition method
void LU_decompose();

void LU_solve();


int main(int argc, char *argv[]) {

    // building A, X, C for the equation [A]{X} = {C}
	//  - first, creating empty matrix & vectors 	
	double **A, *X, *C;
	int i;
	
	A = (double**)malloc(sizeof(int*)*4);
	for(i=0; i<4; i++){
		A[i] = (double*)malloc(sizeof(double)*4);	
	}
	
	X = (double*)malloc(sizeof(double)*4);
	
	C = (double*)malloc(sizeof(double)*4);
	
    //  - then, read [A] and {C} from std input file
    FILE* fp;
    fp = fopen("in_lu.csv", "r");
    
    //Get rid of the subtitles
    while(fgetc(fp) != '\n'){
	}
	
	int n = 0;
	while(fscanf(fp, "%lf,%lf,%lf,%lf,%lf\n", &A[n][0], &A[n][1], &A[n][2], 
		&A[n][3], &C[n]) == 5){
			n++;
	}
	
    // print the original data to check it is read in correctly
    for(i=0; i<4; i++){
    	for(n=0; n<4; n++){
    		printf("%lf ", A[i][n]);	
    	}
    	printf("\n");
	}
	printf("\n");
                     
	// solve the equation
	LU_solve(A);

	// print to solution
	
	
	for(i=0; i<4; i++){
		free(A[i]);	
	}
	
	
	free(A); 
	free(X);
	free(C);
	fclose(fp);
    return 0;
}


// solving, using LU-decomposition
void LU_solve(double **A) {
    // creating matrices [L] and [U] for decoposing [A] into
    double **L, **U;
	int i;
	
	L = (double**)malloc(sizeof(int*)*4);
	U = (double**)malloc(sizeof(int*)*4);
	
	for(i=0; i<4; i++){
		L[i] = (double*)malloc(sizeof(double)*4);
		U[i] = (double*)malloc(sizeof(double)*4);
	}
    
	// creating interim {R}
	double *R;
	R = (double*)malloc(sizeof(double)*4);
	
    // decomposing
	LU_decompose(A, L, U, 4);
  
    //Write your loop for forward substitution
    //Loop over the lower triangular matrix of L

    //Write loop for backward subsitution
    //Loop over upper triangular matrix elements of [U]
    //Write the loop here and obtain [X]
}

void LU_decompose(double **A, double **L, double **U, int n) {
	int i, j, k;
	double sum1=0, sum2=0;
	
    //Loop for first column of L
	for (i=0; i<4; i++){
		L[0][i] = A[0][i]; 	
	}
    
    //Loop for main diagonal elements of [U] and first row
    for(i=0;i<4;i++){
    	U[i][i] = 1;
    	U[i][0] = A[i][0];
    }
    
	//Loop for remaining elements of L and U 
	
	//Spend time figuring this out exactly.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	for(i=1;i<4;i++){
		for(j=1;j<4;j++){
			sum = 0;
			for(k=0;k<j;k++){
				sum1 += L[i][k]*U[k][j]; //for l[
				sum2
			}	
			L[i][j] = A[i][j] 
		}
	}	
	
}

