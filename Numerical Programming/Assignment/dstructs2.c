#include <stdio.h>
#include <stdlib.h>

#define SIZE 419629

typedef struct{
	double rho, u, v, x, y;	
} structure;

int                            
main(int argc, char **argv){
	
	//Open File
	FILE *fp;
	fp = (FILE*)malloc(sizeof(FILE));
	
	fp = fopen(argv[1], "r");
	if(!fp){
		printf("File Open Failed");
		exit(EXIT_FAILURE);	
	}
	
	//Get Past Column Headings
	char c;
	while((c = getc(fp))!='\n'){
	}
	
	
	//======================================================================
	int i, num=1;
	
	structure* array_of_structures;
	/*
	array_of_structures = (structure*)malloc(sizeof(structure));
	*/
	array_of_structures = (structure*)malloc(sizeof(structure)*SIZE);
	
	//Starts off with the space for one, keeps it that was with realloc.
	while(fscanf(fp,"%lf,%lf,%lf,%lf,%lf", &array_of_structures[num-1].rho,
		&array_of_structures[num-1].u, &array_of_structures[num-1].v,
		&array_of_structures[num-1].x, &array_of_structures[num-1].y)==5){
	
		num++;
		
		/*
		array_of_structures = (structure*)realloc(array_of_structures,
			sizeof(structure)*num);
		*/
	}
	printf("%lu of %d cells\n", sizeof(structure)*num, num); 
	
	for(i=SIZE-5; i<SIZE; i++){
		printf("%f %f %f %f %f\n", array_of_structures[i].rho,
		array_of_structures[i].u, array_of_structures[i].v,
		array_of_structures[i].x, array_of_structures[i].y);	
	}
	
	
	free(array_of_structures);
	
	return 0;	
}                                                                            