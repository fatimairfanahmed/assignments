/*
Fatima Irfan
9th April 2022
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel** read_ppm(const char* filename, int* w, int* h) {
  int max = 0;
  char magicNumber[32]; 
  FILE* file = fopen(filename, "rb");
  if (file == NULL) {
    printf("Unable to open file %s\n", filename);
    exit(0);
  }
  char buffer[256];
  fgets(buffer, 256, file);
  strcpy(magicNumber,buffer);
  fgets(buffer, 256, file);
  if (buffer[0] == '#'){
      fgets(buffer, 256, file);
      sscanf(buffer, "%d %d", w, h);
  } 
  else {
    sscanf(buffer, "%d %d", w, h);
  }
  fgets(buffer, 256, file);
  sscanf(buffer, " %d ", &max);
  struct ppm_pixel** output;
  output =  (struct ppm_pixel**)malloc(sizeof(struct ppm_pixel*)*(*h));
  for (int i = 0; i < *h; i++){ 
    output[i] = malloc(sizeof(struct ppm_pixel)*(*w)); //allocating space for each column within each row
  } 
  if(output == NULL) {
    printf("Cannot allocate new memory. Exiting...\n");
    exit(0);
  }

  for (int i = 0; i < *h ; i++){
    for (int j = 0; j < *w ; j++){
        struct ppm_pixel temp;
        fread(&temp, sizeof(struct ppm_pixel), 1, file);
        output[i][j] = temp;
    }
  }
  fclose(file);
  return output;
}

extern void write_ppm(const char* filename, struct ppm_pixel** pxs, int w, int h) {
  FILE* resultFile = fopen(filename, "wb");
  char* header = malloc(sizeof(char)*500);
  sprintf(header, "P6\n%d %d\n255\n", w, h);
  fwrite(header, sizeof(char), strlen(header), resultFile);
  for (int i = 0; i < h; i++){
      fwrite(pxs[i],sizeof(struct ppm_pixel), w, resultFile);
  } 
  free(header);
  header = NULL;
  fclose(resultFile);
}
