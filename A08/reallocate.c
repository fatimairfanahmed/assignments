#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void* top = sbrk(0);
  printf("The initial top of the heap is %p.\n", top);

  for (int i = 0; i < 15; i++){
    int* crrMem = (int*)malloc(sizeof(int)*10);
    if (crrMem == NULL){
      printf("Malloc Failed. exiting.../n");
      exit(0);
    }
    free(crrMem);
  }

  void* current = sbrk(0);
  int allocated = (int)(current - top);
  printf("The current top of the heap is %p.\n", current);
  printf("Increased by %d (0x%x) byte\n", allocated, allocated);

  return 0 ;
}
