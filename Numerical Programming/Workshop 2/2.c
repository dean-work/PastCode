#include <stdio.h>
#include <stdlib.h>

//Constants
#define STACKSIZE 	51
#define INDEX		50

//Typedefs
typedef int* stack_t;

//Function Prototypes
stack_t make_empty_stack();
void push(stack_t stack, int data);
int pop(stack_t stack);
void stackp_f(stack_t stack);

int
main(int argc, char **argv){
	stack_t stack;
	stack = make_empty_stack();
	
	push(stack, 5);
	push(stack, 10);
	push(stack, 40);
	push(stack, 1);
	
	stackp_f(stack);
	
	free(stack);
	return 0;
}

//Functions
stack_t make_empty_stack(){
	//My convention is that the last array element is the index.
	stack_t tmp = NULL;
	tmp = malloc(STACKSIZE*sizeof(int));
	tmp[INDEX] = 0;
	return tmp;
}

void push(stack_t stack, int data){
	stack[stack[INDEX]] = data;
	stack[INDEX]++;
}

int pop(stack_t stack){
	int tmp = stack[stack[INDEX]];
	stack[INDEX]--;
	stack[stack[INDEX]] = 0;
	
	return tmp;
}

void stackp_f(stack_t stack){
	int i;
	
	for (i=0; i<stack[INDEX]; i++){
		printf("%d _ ", stack[i]);
	}
	printf("\n");
}