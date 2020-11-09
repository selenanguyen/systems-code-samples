// You should start with your previous assignment's mymalloc.c :)
//
//
#include <sched.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include <stdio.h> // Any other headers we need here
#include <malloc.h> // We bring in the old malloc function
// NOTE: You should remove malloc.h, and not include <stdlib.h> in your final implementation.

#define MAX_SIZE 1000000

int counter = 0;
typedef struct block{
	size_t size; // How many bytes beyond this block have been allocated in the heap
	struct block* next; // Where is the next block in your linked list
	int free; // Is this memory free?
	int debug; // (optional) Perhaps you can embed other information--remember, you are the boss!
}block_t;

#define BLOCK_SIZE sizeof(block_t)

//need to define a template for this function that gets the current cpu
int sched_getcpu();

// one free list per CPU
block_t* freelists; 

// one lock per CPU
pthread_mutex_t* mutexArr;

//global lock for initializing list
pthread_mutex_t globalLock = PTHREAD_MUTEX_INITIALIZER;

// initialize lists at the beginning of the program
void __attribute__((constructor)) initializeLists();

//helper function defined below -- need signature here so it compiles
block_t* getBestFit(size_t s, block_t listHead);

//number of CPUs
int numThreads;

//helper function to print out freelist, which is the linked list that contains the allocated memory
void printList() {
	int i;
	for (i = 0; i < numThreads; i++) {
		block_t *current = &freelists[i];
		while (current != NULL) {
			block_t *b = (block_t *)current;
			printf("CPU: %d (addr: %p), Size: %zu, Free: %d, Debug: %d, Next: %p\n", i, b, b->size, b->free, b->debug, b->next);
			if (current != current->next) {
				current = current->next;
			} else {
				break;
			}
		}
	}
}

//initializes the freelists array with 48 free lists; one for each CPU on the ccs login machine
//also initializes the locks array with one lock per CPU
void initializeLists(){
	numThreads = get_nprocs();
	pthread_mutex_lock(&globalLock);	
	
	freelists = sbrk(0);
	sbrk(sizeof(block_t) * numThreads);
	mutexArr = sbrk(0);
	sbrk(sizeof(pthread_mutex_t) * numThreads);
	
	int i;
	printf("Initializing lists...\n");
	for (i = 0; i < numThreads; i++) {
		freelists[i].free = -1;
		freelists[i].debug = -1;
		freelists[i].size = 0;
		freelists[i].next = NULL;
		pthread_mutex_init(&mutexArr[i], NULL);
	}
	pthread_mutex_unlock(&globalLock);
}

void* mymalloc(size_t s){
	if (s <= 0) {
		printf("Cannot malloc 0 or less\n");
		perror("malloc 0 or less\n");
		exit(1);
	}
	// cpu number (0 - 47)
	int cpu = sched_getcpu();

	pthread_mutex_t lock = mutexArr[cpu];
	pthread_mutex_lock(&lock);
	block_t* listHead = &freelists[cpu];

	printf("malloc %zu bytes\n",s);
	int useSbrk = 0;
	if (s < 4096) {
		useSbrk = 1;
	}

	// Find the best-fit memory block in the current list
	block_t* b = getBestFit(s, *listHead);
	if (b == NULL) {
		printf("Did not find best fit for CPU: %d\n", cpu);
		void *p;
		if (useSbrk) {
			p = sbrk(0);
			sbrk(s + BLOCK_SIZE);
		}
		else {
			p = mmap(NULL, s + BLOCK_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
		}
		if (p == (void*) -1) {
			//printList();
			pthread_mutex_unlock(&lock);
			return NULL;
		}
		block_t* blk = (block_t*) p;
		blk->size = s;
		blk->free = 0;
		blk->next = NULL;
		blk->debug = counter;
		counter++;
		block_t* currentP = listHead;
		// Adding the new memory block to the list
		while(currentP != NULL) {
			if(currentP->next == NULL) {
				currentP->next = blk;
				break;
			}
			currentP = currentP->next;
		}
		//printList();
		pthread_mutex_unlock(&lock);
		return (void*)(p + BLOCK_SIZE);
	}
	printf("Found best fit for CPU: %d\n", cpu);
	b->free = 0;
	//need to cast the block_t type before adding block size to it otherwise it breaks :)))))
	void* casted = (void*)b;
	//printList();
	pthread_mutex_unlock(&lock);
	return (void*)(casted + BLOCK_SIZE);
}

void* mycalloc(size_t nmemb, size_t s){
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
	//printList();
	printf("Freed some memory\n"); //: %p\n", ptr - BLOCK_SIZE);
	int cpu = sched_getcpu();
	pthread_mutex_t lock = mutexArr[cpu];
	pthread_mutex_lock(&lock);
	
	block_t* b = (block_t*)(ptr - BLOCK_SIZE);
	if (b == NULL) {
		printf("Invalid block\n");
		perror("free");
		exit(1);
	}
	b->free = 1;
	//printf("Free = %d\n", b->free);
	block_t* nextBlock = (block_t*)(b->next);
	void* nextBlockAddr = (void*)(b->next);
	void* endOfCurrentBlock = (void*)(b->size + ptr);
	if (b->next == NULL) {
		//printList();
		pthread_mutex_unlock(&lock);
		return;
	}
	// if the next block is free and the blocks' addresses are adjacent,
	// merge these blocks
	if (nextBlock->free == 1 && endOfCurrentBlock == nextBlockAddr) {
		printf("Merging blocks\n");
		b->size = b->size + nextBlock->size + BLOCK_SIZE;
		b->next = nextBlock->next;
	}
	pthread_mutex_unlock(&lock);
	//printList();
}
	
//this function loops through the linked list and finds the smallest available spot for 
//the given size. If there are more than 1 of the same size, it keeps the earlier one.
//**there are commented print statements that tell whether blocks are being reused or not.
//this function returns null if there is no best fit found, and that signals to mymalloc
//that it is necessary to run sbrk.
block_t* getBestFit(size_t s, block_t listHead) {
	block_t* currentP = &listHead;
	size_t bestSizeSoFar = MAX_SIZE;
	block_t* bestFitSoFar = NULL;
	while(currentP != NULL) {
		if(currentP->size >= s && currentP->free == 1 && currentP->size < bestSizeSoFar) {	
			//printf("reusing block\n");
			bestSizeSoFar = currentP->size;
			bestFitSoFar = currentP;
		}
		currentP = currentP->next;
	}

	// Splitting the block into two if we have enough space
	if (bestFitSoFar != NULL && bestFitSoFar->size > s + BLOCK_SIZE) {
		printf("Fragmentation occurring\n");
		void *voidBestFitSoFar = (void*)bestFitSoFar;
		void *newBlockV = (void*)(voidBestFitSoFar + BLOCK_SIZE + s);
		block_t* newBlock = (block_t*) newBlockV;	
		newBlock->next = bestFitSoFar->next;
		bestFitSoFar->next = newBlock;
		newBlock->size = bestFitSoFar->size - s - BLOCK_SIZE;
		newBlock->free = 1;
		newBlock->debug = 999;
		bestFitSoFar->size = s;
	//	printList();
	}
	//if (bestFitSoFar == NULL) {
	//	printf("no best fit found\n");
	//}

	return bestFitSoFar;
}
