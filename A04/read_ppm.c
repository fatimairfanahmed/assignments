/*
 * Program Name : read_ppm.c 
 * CS223 - Spring 2022
 * Author: Fatima Irfan
 * Date: 02/19/2022
 * Read and stored a PPM file
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel** read_ppm(const char* filename, int* w, int* h) {
  int max = 0;
  char magicNumber[32]; 
  FILE* file = fopen(filename, "r");
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
  } else {
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
        unsigned char r, g, b;
        fscanf(file, " %hhu %hhu %hhu", &r, &g, &b);
        output[i][j].red = r;
        output[i][j].green = g;
        output[i][j].blue = b;
      }
  }
  fclose(file);
  return output;
}

