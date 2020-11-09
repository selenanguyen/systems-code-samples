// Implement parallel quicksort here
// TO RUN: gcc -std=c99 -fopenmp myqsort.c -o myqsort
// 	       ./myqsort FILENAME
// (added data.txt as example file)
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void quicksort(int *arr, int low, int high);
int partition(int *arr, int l, int h);
FILE* getFile(char *filename);

int main(int argc, char *argv[]) {
	FILE* fp = getFile(argv[1]);
    int temp = 0;
    int n = 0; // number of nums

    while(fscanf(fp, "%d", &temp) != EOF) {
		n = n + 1;
	}

    fclose(fp);
    fp = getFile(argv[1]);

    int arr[n];
    int i = 0;
    while(fscanf(fp, "%d", &arr[i]) != EOF) {
		i++;
	}

    // Print unsorted array
    // printf("UNSORTED:\n");
	// for(i = 0; i < n; i++){
    //     	printf("%d ", arr[i]);
    // }
    // printf("\n");

    // Sort the array
    quicksort(arr, 0, n - 1);

    // Print the sorted array
    printf("\nSorted integers:\n");
	for(i = 0; i < n; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}

// Applies quicksort
// low = starting index; high = ending index
void quicksort(int *arr, int low, int high)
{
    int p; // array partition
    if(low < high){ 
        // get the partition
        p = partition(arr, low, high); 
        #pragma omp parallel sections 
        { 
            #pragma omp section 
            {
                quicksort(arr, low, p - 1);  // quicksort below partition
            }
            #pragma omp section 
            {
                quicksort(arr, p + 1, high); // quicksort above partition
            }
        }
    }
}

// Partitions the array
// Returns the index of the partition
int partition(int *arr, int l, int h)
{
    int k = arr[h];
    int arr_lt[h-l], arr_gt[h-l];
    int lt = 0;
    int gt = 0;
    int i, j;
#pragma omp parallel for
    for (i = l; i < h; i++) {
        if (arr[i] < arr[h]) {
            arr_lt[lt] = arr[i];
            lt++;
        }
        else {
            arr_gt[gt] = arr[i];
            gt++;
        }   
    }   
    for (i = 0; i < lt; i++) {
        arr[l + i] = arr_lt[i];
    }   
    arr[l + lt] = k;
    for (j = 0; j < gt; j++) {
        arr[lt + l + j + 1] = arr_gt[j];
    }   
    return lt + l;
}

// Returns a pointer to the file to read
FILE* getFile(char *filename) {
    FILE* fp;
    fp  = fopen(filename, "r");
	if (fp == NULL) {
        printf("%s not found.\n", filename);
        fprintf(stderr,"Program terminating now...");
		exit(EXIT_FAILURE);
    }
    return fp;
}