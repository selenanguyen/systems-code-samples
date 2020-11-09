#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define NUM_THREADS 5

// Here is some data structure that will be shared.
// It is an array of strings, but nothing has been allocated yet.
char** shared_data;

//
void *some_thread1(void *index_to_shared_data){
	// Allocates some memory for an individual string
	char* str = "some_thread1";
	// Generate some random data/string to put in shared_data
	// e.g. shared_data[index_to_shared_data] "some_thread1";
	char* str2 = (char*)malloc(12 * sizeof(char));
	shared_data[(int)index_to_shared_data] = (char*)malloc(12 * sizeof(char));
	strcpy(shared_data[(int)index_to_shared_data], str);
	// Terminate thread
	return NULL;
}

void *some_thread2(void *index_to_shared_data){
	// Allocates some memory for an individual string
	char* str = "some_thread2";
	char* str2 = (char*)malloc(12 * sizeof(char));
	// Generate some random data/string to put in shared_data
	// e.g. shared_data[index_to_shared_data] "some_thread2";
	shared_data[(int)index_to_shared_data] = (char*)malloc(12 * sizeof(char));
	strcpy(shared_data[(int)index_to_shared_data], str);
	// Terminate thread
	return NULL;
}

int main(){
	pthread_t tids[NUM_THREADS];
	// (1) Malloc for some size of your shared data (i.e. how many strings will you have)
	shared_data = (char**)malloc(NUM_THREADS * sizeof(char*));
	// (2) Launch some number of threads (perhaps with two or more different functions)
	int i;
	for (i = 0; i < NUM_THREADS; ++i) {
		if (i % 2 == 0) {
			pthread_create(&tids[i], NULL, some_thread2, (void*)i);
		} else {
			pthread_create(&tids[i], NULL, some_thread1, (void*)i);
		}
	}
	// (3) Join some number of threads
	for(i = 0; i < NUM_THREADS; ++i){
		pthread_join(tids[i], NULL);
	}
	// (4) Print the results of shared data (i.e. this is done sequentially)
	for(i = 0; i < NUM_THREADS; ++i){
		printf("%s\n", shared_data[i]);
	}
	// (5) Cleanup your program
	// 'free shared_data'
	for (i = 0; i < NUM_THREADS; ++i) {
		free(shared_data[i]);
	}
	free(shared_data);
}
