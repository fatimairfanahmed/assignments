#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int memInUse;
  struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  if (size == 0){
    return NULL;
  }
  struct chunk* next = flist;
  struct chunk* prev = NULL;

  while (next != NULL) {
    if (next->size >= size) {
      if (prev != NULL) {
        prev->next = next->next;
      } 
      else {
        flist = next->next;
      }
      next->memInUse = size; //we are putting it equal to the size it contains, next is the curernt block we have selected
      return (void*)(next + 1);
    } 
    else {
      prev = next ;
      next = next->next ;
    }
  }

  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *) -1) {
    return NULL;
  } else {
    struct chunk* cnk = (struct chunk*) memory;
    cnk->size = size;
    cnk->memInUse = size;
    return (void*) (cnk + 1);
  }
}

void free(void *memory) {
  if (memory != NULL ){
    struct chunk *cnk = (struct chunk *)((struct chunk *)memory - 1);
    cnk->next = flist;
    flist = cnk;
  }
  return;
}

void fragstats(void* buffers[], int len) {
  int totalInUseChunks =  0;
  int totalMemUnUsed = 0;
  int smallMemUnUsed = 9999;
  int largeMemUnUsed = 0;
  int avgMemUnUsed = 0;
  int totalFreeChunks = 0;
  int smallMemFree =    999999;
  int largeMemFree =    0;
  int totalMemFree =    0;
  int avgMemFree =      0;

  struct chunk* freeNext = flist; 
  while (freeNext != NULL){
    totalFreeChunks = totalFreeChunks + 1;
    totalMemFree = totalMemFree + freeNext->size;
    if (freeNext->size < smallMemFree){
      smallMemFree = freeNext->size;
    }
    if (freeNext->size > largeMemFree){
      largeMemFree = freeNext->size;
    }
    freeNext = freeNext->next;
  }
  avgMemFree = totalMemFree/totalFreeChunks;

  for (int i = 0; i < len; i++){
    if (buffers[i] != NULL){
      totalInUseChunks = totalInUseChunks + 1;
      struct chunk* crrChunkInUse = (struct chunk*)((struct chunk*)buffers[i] - 1);
      int crrUnusedMemory = crrChunkInUse->size - crrChunkInUse->memInUse;
      totalMemUnUsed = totalMemUnUsed + crrUnusedMemory;
      if (crrUnusedMemory > largeMemUnUsed){
        largeMemUnUsed = crrUnusedMemory;
      }
      if (crrUnusedMemory < smallMemUnUsed){
        smallMemUnUsed = crrUnusedMemory;
      }
    }
  }
  avgMemUnUsed = totalMemUnUsed/totalInUseChunks;

  printf("Total blocks: %d Free: %d Used: %d\n", totalFreeChunks + totalInUseChunks, totalFreeChunks, totalInUseChunks);
  printf("Internal unused: total: %d average: %d smallest: %d largest: %d\n",totalInUseChunks,avgMemUnUsed,smallMemUnUsed,largeMemUnUsed);
  printf("External unused: total: %d average: %d smallest: %d largest: %d\n", totalMemFree, avgMemFree, smallMemFree, largeMemFree);


}

