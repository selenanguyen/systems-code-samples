#include <stdio.h>
#include <malloc.h>


int main(){

  // Allocate several different uniquely sized blocks
  // Ideally your allocator will be able to re-use many
  // of these blocks.
  for(int i= 0; i < 100; i++){
      int* data =  (int*)calloc(2, 5);    
      int* data1 = (int*)calloc(4, 5);
      int* data2 = (int*)calloc(6, 5);
      int* data3 = (int*)calloc(8, 5);
      int* data4 = (int*)calloc(10, 5);
      
      free(data);
      free(data1);
      free(data2);
      free(data3);
      free(data4);
  }

  return 0;
}
