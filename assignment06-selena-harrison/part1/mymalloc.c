#include <stdio.h> // Any other headers we need here
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h> // We bring in the old malloc function
                    // NOTE: You should remove malloc.h, and not include <stdlib.h> in your final implementation.
#define MAX_SIZE 1000000

void* HEAD = NULL;
int counter = 0;
typedef struct block{
	size_t size; // How many bytes beyond this block have been allocated in the heap
	struct block* next; // Where is the next block in your linked list
	int free; // Is this memory free?
        int debug; // (optional) Perhaps you can embed other information--remember, you are the boss!
}block_t;

#define BLOCK_SIZE sizeof(block_t)

//helper function defined below -- need signature here so it compiles
block_t* getBestFit(size_t s);

//helper function to print out HEAD, which is the linked list that contains the allocated memory
void printHead() {
	block_t* current = (block_t*)HEAD;
	while (current != NULL) {
		block_t* b = (block_t*)current;
		printf("Size: %zu, Free: %d, Debug: %d\n", b->size, b->free, b->debug);
		current = current->next;
	}
}

//mallocs memory for the given size and returns the pointer to that area
//if there has been no memory allocated yet, sets the HEAD to the first allocated memory
//otherwise, looks to see if the current size could fit in a free location
//-- if so: returns that location
//-- if not: allocates more memory for the given size
void* mymalloc(size_t s){
	printf("malloc %zu bytes\n", s);
	// Sets the HEAD to the first memory block
	if (HEAD == NULL) {
		void *p = sbrk(0);
        	if (p == (void*) -1) {
                	return NULL;
        	}
		sbrk(s + BLOCK_SIZE);
		HEAD = p;
		block_t* b = (block_t*) p;
		b->size = s;
		b->free = 0;
		b->next = NULL;
		b->debug = counter;
		counter++;
		return (void*)(p + BLOCK_SIZE);
	}
	
	// Find the best-fit memory block in the current list
	block_t* b = getBestFit(s);
	if (b == NULL) {
		void *p = sbrk(0);
		if (p == (void*) -1) {
			return NULL;
        	}
		sbrk(s + BLOCK_SIZE);
		block_t* blk = (block_t*) p;
		blk->size = s;
		blk->free = 0;
		blk->next = NULL;
		blk->debug = counter;
		counter++;
		block_t* currentP = HEAD;
		// Adding the new memory block to the list
		while(currentP != NULL) {
			if(currentP->next == NULL) {
				currentP->next = blk;
				break;
			}
			currentP = currentP->next;
		}
		return (void*)(p + BLOCK_SIZE);
	}
	b->free = 0;
	//need to cast the block_t type before adding block size to it otherwise it breaks :)))))
	void* casted = (void*)b;
	return (void*)(casted + BLOCK_SIZE);
}                    

//malloc the size of nmemb * s, then set that area of memory to be all zeros.
void* mycalloc(size_t nmemb, size_t s){
	if (!nmemb || !s) {
		return NULL;
	}
	size_t size = nmemb * s;
	void* b = mymalloc(size);
	memset(b,0,size); 
	printf("calloc %zu bytes\n", s);
	return b;
}

//sets the block_t associated with this pointer to be free
void myfree(void *ptr){
	printf("Freed some memory\n");
	block_t* b = (block_t*)(ptr - BLOCK_SIZE);
	if (b == NULL) {
		perror("free");
		exit(1);
	}
	b->free = 1;
}

//this function loops through the linked list and finds the smallest available spot for 
//the given size. If there are more than 1 of the same size, it keeps the earlier one.
//**there are commented print statements that tell whether blocks are being reused or not.
//this function returns null if there is no best fit found, and that signals to mymalloc
//that it is necessary to run sbrk.
block_t* getBestFit(size_t s) {
	block_t* currentP = HEAD;
	size_t bestSizeSoFar = MAX_SIZE;
	block_t* bestFitSoFar = NULL; // do we need to allocate memory for this?
	while(currentP != NULL) {
		if(currentP->size >= s && currentP->free == 1 && currentP->size < bestSizeSoFar) {	
			//printf("Reusing block\n");	
			bestSizeSoFar = currentP->size;
			bestFitSoFar = currentP;
		}
		currentP = currentP->next;
	}
	//if (bestFitSoFar == NULL) {
	//	printf("no best fit found\n");
	//}
	return bestFitSoFar;
}
