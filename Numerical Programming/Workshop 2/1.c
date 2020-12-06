#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATASIZE 1

//Type Declarations
typedef struct node_s node_t; //node type is defined as the node structure.

struct node_s{
	int data; 
	node_t* next; //next on next node.
};

//Function Prototypes
node_t* create_chain(int data);
void print_chain(node_t *x);
void push(int data, node_t* root);
void free_all(node_t* root);
void insert_after(node_t* root, node_t* n, int data);
node_t* find_node(node_t* root, int data);


int
main (int argc, char **argv){
	
	//Create and declare node
	node_t* starting_node;
	starting_node = create_chain(5);
	
	push(11, starting_node);
	push('c', starting_node);
	push(13, starting_node);
	push(50, starting_node);
	
	print_chain(starting_node);
	printf("\n");
	
	insert_after(starting_node, find_node(starting_node, 13), 14);
	
	print_chain(starting_node);
	printf("\n");
	
	free_all(starting_node);
	return 0;
}




// Functions
node_t* create_chain(int data){
	node_t* tmp = NULL;	
	tmp = malloc(sizeof(int) + sizeof(node_t*)); //malloc the structures  
	
	tmp->data = data;	
	tmp->next = NULL;
	
	return tmp;                           
}

void print_chain(node_t *x){
	printf("%d\n",	x->data);
	if (x->next)
		print_chain(x->next);
}

void push(int data, node_t* root){
	if (!(root->next)){
		root->next = create_chain(data);
	} else {
		push(data, root->next);
	}		
}

void free_all(node_t* root){
	if (root->next) {
		free_all(root->next);	
	} 
	
	free(root);
}

node_t* find_node(node_t* root, int data){
	if(root->data == data){
		return root; 
	} else if (root->next != NULL){
		return find_node(root->next, data); //Ensure to return recursions.
	} else {
		return NULL;	
	}
	printf("How the fuck did it get here!\n");
	return NULL;
}

void insert_after(node_t* root, node_t* n, int data){
	//Function works by moving root down the cain, when it equals n, returns
	//n's address.
	
	if ((root == NULL) || (n == NULL)) { //Dereference a null == seg. fault
		printf("Root is %p n is %p\n", root, n);
		exit(EXIT_FAILURE);
	}
	//1. Scale down to node.
	else if ((n!=root)&&(n->next != NULL)) { //Check it isn't null, seg fault.
		insert_after(root->next, n, data);	
	}
	else if (n==root) {
		
	//2. Create new node.
	//3. Replace the previous node with the new node. Keep prev node address.
		node_t* old = n->next;
		n->next = create_chain(data);
	//4. Tell the new node, to point to the old node.
		(n->next)->next = old;	
		

	} //In case of bull shittery
	else if ((n!=root)&&(n->next == NULL)) {
		printf("/nWe didn't find the fucking node!/n");
		exit(EXIT_FAILURE);
	} 
	
}