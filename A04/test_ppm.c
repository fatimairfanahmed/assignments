/*
 * Program Name : test_ppm.c 
 * CS223 - Spring 2022
 * Author: Fatima Irfan
 * Date: 02/19/2022
 * Tests the read_ppm function
 */
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  int w = 0;
  int h = 0;
  struct ppm_pixel** output = read_ppm("feep-ascii.ppm", &w , &h);
  int counter = 0;
  printf("Testing file feep-ascii.ppm: %d %d\n",w,h);
  for (int k = 0; k < h; k++) {
    for (int j = 0; j < w; j++) {
      printf(" (%hhu %hhu %hhu) ", output[k][j].red, output[k][j].green, output[k][j].blue);
      counter++;
      if (counter % w == 0 ){
        printf("\n");
      }
    }
  }
  for (int i = 0; i < h; i++){
    free(output[i]);
  } 
  free(output);
  output = NULL;
  return 0;
}

