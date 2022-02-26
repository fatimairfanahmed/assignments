/*
 * Program Name : glitch.c 
 * CS223 - Spring 2022
 * Author: Fatima Irfan
 * Date: 02/25/2022
 * RGlitches a PPM File
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if(argc != 2){
    printf("Please input a file\n");
      return 1;
  }
  int w = 0;
  int h = 0;
  char* inputFile = argv[1];
  struct ppm_pixel** output = read_ppm(inputFile, &w , &h);
  printf("Reading %s with width %d and height %d\n",inputFile, w,h);
  for (int i = 0; i < h ; i++){
    for (int j = 0; j < w ; j++){
      output[i][j].blue = output[i][j].red << (rand() % 2);
      output[i][j].red = output[i][j].red << (rand() % 2);
      output[i][j].green = output[i][j].red << (rand() % 2);
    }
  }
  char* newFile = strtok(inputFile, ".");
  strcat(newFile, "-glitch.ppm");
  printf("Writing file: %s\n", newFile);
  write_ppm(newFile, output, w , h);
  for (int i = 0; i < h; i++){
    free(output[i]);
  } 
  free(output);
  output = NULL;
  newFile = NULL;
  return 0;
}
