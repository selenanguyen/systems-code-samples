// You should start with your previous assignment's mymalloc.c :)
//
//
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <stdio.h> // Any other headers we need here
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

void* mymalloc(size_t s){

	printf("malloc %zu bytes\n",s);
	int useSbrk = 0;
	if (s < 4096) {
		useSbrk = 1;
	}
	// TODO: abstract out memory allocation (sbrk & mmap)
	// Sets the HEAD to the first memory block
	if (HEAD == NULL) {
		void *p;
		if (useSbrk) {
			p = sbrk(0);
			sbrk(s + BLOCK_SIZE);
		} 
		else {
			p = mmap(NULL, s + BLOCK_SIZE, PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
		}
		if (p == (void*) -1 ) {
			return NULL;
		}
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
		void *p;
		if (useSbrk) {
			p = sbrk(0);
			sbrk(s + BLOCK_SIZE);
		}
		else {
			p = mmap(NULL, s + BLOCK_SIZE, PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
		}
		if (p == (void*) -1) {
			return NULL;
        }
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

void* mycalloc(size_t nmemb, size_t s){

	// In your solution no calls to calloc should be made!
						// Determine how you will request memory :)

	if (!nmemb || !s) {
		return NULL;
	}
	size_t size = nmemb * s;
	void* b = mymalloc(size);
	memset(b,0,size); 
	printf("calloc %zu bytes\n", s);
	return b;
}
	
void myfree(void *ptr){
	printf("Freed some memory\n");
	block_t* b = (block_t*)(ptr - BLOCK_SIZE);
	if (b == NULL) {
		perror("free");
		exit(1);
	}
	b->free = 1;
	block_t* nextBlock = (block_t*)(b->next);
	void* nextBlockAddr = (void*)(b->next);
	void* endOfCurrentBlock = (void*)(b->size + ptr);

	// if the next block is free and the blocks' addresses are adjacent,
	// merge these blocks
	if (nextBlock->free == 1 && endOfCurrentBlock == nextBlockAddr) {
		b->size = b->size + nextBlock->size + BLOCK_SIZE;
		b->next = nextBlock->next;
	}
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

	// Splitting the block into two if we have enough space
	if (bestFitSoFar != NULL && bestFitSoFar->size > s + BLOCK_SIZE) {
		void *newBlockV = bestFitSoFar + BLOCK_SIZE + bestFitSoFar->size;
		block_t* newBlock = (block_t*) newBlockV;
		newBlock->next = bestFitSoFar->next;
		bestFitSoFar->next = newBlock;
		newBlock->size = bestFitSoFar->size - s - BLOCK_SIZE;
		newBlock->free = 1;
		bestFitSoFar->size = s;
	}

	//if (bestFitSoFar == NULL) {
	//	printf("no best fit found\n");
	//}
	return bestFitSoFar;
}