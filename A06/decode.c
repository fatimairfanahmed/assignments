#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if(argc != 2){
    printf("Usage\n");
      exit(0);
  }
  int w = 0;
  int h = 0;
  char* inputFile = argv[1];
  struct ppm_pixel** output = read_ppm(inputFile, &w , &h);
  printf("Reading %s with width %d and height %d\n",inputFile, w,h);
  int totalChar = 0;
  totalChar = (w*h*3)/8 - 1;
  printf("Max number of characters in the image: %d\n", totalChar - 1);
  unsigned char message = 0;
  int counter = 0;
  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j ++){
      for (int k = 0; k < 3; k++){
        if (counter < 8) {
          unsigned char val = output[i][j].colors[k] & 0x1;
          val = val << (7 - counter);
          message = message | val;
          counter++;
        }
        if (counter == 8) {
          printf("%c", message);
          counter = 0;
          message = 0;
        }
      }
    }
  }
  printf("\n");
  for (int i = 0; i < h; i++){
    free(output[i]);
  } 
  free(output);
  output = NULL;
  return 0;
}

