#include <stdio.h>
#include <stdlib.h>

#define N 2 //Rows
#define M 4 //Cols

//Type definitions
typedef int** matrix;

//Function Prototypes
matrix create_matrix();
void free_matrix(matrix matrix);
void free_matrix(matrix matrix);

int
main(int argc, char **argv){
	matrix m = NULL;
	m = create_matrix();
	
	int i;	
	for(i=0; i<N; i++){
		scanf("%d %d %d %d", &m[i][0], &m[i][1], &m[i][2], &m[i][3]);
	}
	
	free_matrix(m);
	
	return 0;	
}

matrix create_matrix(){
	matrix tmp = NULL;
	tmp = (matrix)malloc(sizeof(int*)*N);
	
	int i;
	for (i=0; i<M; i++){
		tmp[i] = (int*)malloc(sizeof(int)*M);	
	}
	
	return tmp;
}

void free_matrix(matrix matrix){
	int i;
	for (i=0; i<M; i++){
		free(matrix[i]);	
	}
	
	free(matrix);
}