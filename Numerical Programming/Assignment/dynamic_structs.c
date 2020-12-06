#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int a, b, c;	
} structure;

int                            
main(int argc, char **argv){
	int i, num=1;
	
	structure* array_of_structures;
	array_of_structures = (structure*)malloc(sizeof(structure));
	
	//Starts off with the space for one, keeps it that was with realloc.
	while(scanf("%d %d %d", &array_of_structures[num-1].a,
		&array_of_structures[num-1].b, &array_of_structures[num-1].c)==3){
		printf("Size of %lu\n", sizeof(structure)*num);
		
		num++;
		
		array_of_structures = (structure*)realloc(array_of_structures,
			sizeof(structure)*num);
		
	}
	
	for(i=0; i<num; i++){
		printf("%d %d %d\n", array_of_structures[i].a, array_of_structures[i].b,
			array_of_structures[i].c);	
	}
	
	free(array_of_structures);
	
	return 0;	
}                                                                            