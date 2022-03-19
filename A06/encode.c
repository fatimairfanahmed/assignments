#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

void charToBin(char curr, unsigned char* crrChar){
  for (int i = 0; i < 8; i++){
    crrChar[i] = curr >> (7 - i) & 0x01;
  }
}

int main(int argc, char** argv) {
  if(argc != 2){
    printf("Usage\n");
      exit(0);
  }
  int w = 0; int h = 0;
  char* inputFile = argv[1];
  struct ppm_pixel** output = read_ppm(inputFile, &w , &h);
  printf("Reading %s with width %d and height %d\n",inputFile, w,h);  
  int totalChar = (w*h*3)/8 - 1;
  printf("Max number of characters in the image: %d\n", totalChar);
  char* message = malloc(sizeof(char)*totalChar + 1);
  printf("Enter a phrase: ");
  scanf("%[^\n]s", message);
  int counter = 0; 
  int index = 0;
  unsigned char crrChar[8];
  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
      for (int k = 0; k < 3; k++){
        if (counter == 8) {
          index++;
          counter = 0;
        }
        if (index > strlen(message)) {
          i = 10000; j = 10000; k = 10000;
          break;
        }
        charToBin(message[index], crrChar);
        if (counter < 8) {
          output[i][j].colors[k] = output[i][j].colors[k] & 0xfe;
          if (crrChar[counter] == 1){
            output[i][j].colors[k] = output[i][j].colors[k] | 0x1;
          }
          counter++;
        }
      }
    }
  }

  char* newFile = strtok(inputFile, ".");
  strcat(newFile, "-encoded.ppm");
  printf("Writing file: %s\n", newFile);
  write_ppm(newFile, output, w , h);
  for (int i = 0; i < h; i++){
    free(output[i]);
  } 
  free(output);
  free(message);
  output = NULL;
  newFile = NULL;
  message = NULL;
  return 0;
}

