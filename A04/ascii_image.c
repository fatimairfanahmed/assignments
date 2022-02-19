/*
 * Program Name : ascii_image.c 
 * CS223 - Spring 2022
 * Author: Fatima Irfan
 * Date: 02/19/2022
 * Produces an ASCII Image
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

char assignChar(int intense){
  if (intense < 26){
    return '@';
  }
  else if (intense < 51){
    return '#';
  }
  else if (intense < 76){
    return '%';
  } 
  else if (intense < 101){
    return '*';
  }
  else if (intense < 126){
    return 'o';
  }
  else if (intense < 151){
    return ';';
  }
  else if (intense < 176){
    return ':';
  }
  else if (intense < 201){
    return ',';
  }
  else if (intense < 226){
    return '.';
  }
  else if (intense < 256){
    return ' ';
  }
  else {
    return'$'; //$ specifies error
  }
}

int main(int argc, char** argv) {
  if (argv[1] == NULL) {
    printf("usage  \n");
    exit(0);
  }
  int w = 0;
  int h = 0;
  struct ppm_pixel** output = read_ppm(argv[1], &w , &h);
  printf("Reading %s with width %d and height %d\n", argv[1],w,h);

  for (int k = 0; k < h; k++) {
    for (int j = 0; j < w; j++) {
      double total = output[k][j].red + output[k][j].green + output[k][j].blue;
      double intensity = total/3;
      char crr = assignChar(intensity);
      printf("%c",crr);
    }
    printf("\n");
  }
  for (int i = 0; i < h; i++){
    free(output[i]);
  } 
  free(output);
  output = NULL;

  return 0;

}

