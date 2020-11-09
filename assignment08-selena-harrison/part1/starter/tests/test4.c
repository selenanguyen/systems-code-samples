// Basic test

#include <stdio.h>
#include <malloc.h>

#define ARRAY_ELEMENTS 1023

int main(){

  // Allocate some data
  int* data = (int*)calloc(sizeof(int), ARRAY_ELEMENTS);
  
  // Do something with the data
  int i=0;
  for(i =0; i < ARRAY_ELEMENTS; i++){
    data[i] = i;
  }
	
  // Free the data
  free(data);

  return 0;
}
