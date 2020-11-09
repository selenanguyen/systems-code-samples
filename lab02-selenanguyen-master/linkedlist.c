// Modify this file
// compile with: gcc linkedlist.c -o linkedlist

#include <stdio.h>
#include <stdlib.h> // Don't forget to include this library if you use malloc/free

// Create your node_t type here
typedef struct node{
	int data;
	struct node* next;
}node_t;

// Write your functions here
// There should be 1.) create_list 2.) print_list 3.) free_list
// You may create as many helper functions as you like.

// Creates list by reading "data.txt" file
node_t* create_list() {
	FILE* myFile;
        myFile = fopen("./data.txt", "r");
        if(NULL == myFile){
                fprintf(stderr,"data.txt not found, did you run data.sh?");
                fprintf(stderr,"\nProgram terminating now...");
                exit(1);
        }
	
	node_t* head = malloc(sizeof(node_t)); // allocating memory
	char str[255]; // file line being read
	node_t* prevNode = head;
        while(fscanf(myFile, "%s", str) != EOF) {
		node_t* node;
		node = malloc(sizeof(node_t));
		node->data = atoi(str);
		node->next = NULL;
		prevNode->next = node;
		prevNode = node;
        }
        fclose(myFile);
	node_t* newHead = head->next;
	free(head);
	return newHead;
}

// Prints the given list of Red Sox wins
// Great year: > 80 wins
// Bad year: < 75 wins
// Okay year: Between 75 and 80 wins (inclusive)
void print_list(node_t* list) {
	int i = 0;
	node_t* currentNode = list;
	while(currentNode != NULL) {
		int wins = currentNode->data;
		printf("%d wins. ", wins);
		if(wins > 80) {
			printf("Great year!\n");
		}
		else if(wins < 75) {
			printf("Bad year...\n");
		}
		else {
			printf("They did okay.\n");
		}
		i = i + 1;
		currentNode = currentNode->next;
	} 
}

// Frees allocated memory in the given list
void free_list(node_t* list) {
	node_t* currentNode = list;
	while(currentNode != NULL) {
		node_t* nodeTemp = currentNode->next;
		currentNode->next = NULL;
		free(currentNode);
		currentNode = nodeTemp;
	}
}

int main(){
	// creates a list pointer
	node_t* list = create_list();	
	// prints the list
	print_list(list);
	// frees the list 
	free_list(list);
	return 0;
}
