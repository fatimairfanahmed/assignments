/*
 * Program Name : repeat.c
 * Author: Fatima Irfan
 * Date: 2/4/2022
 * Repeat a Word
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char input[32];
  int repeat;
  printf("Enter a word: \n");
  scanf("%s",input);
  int len = strlen(input);
  printf("Enter a count: \n");
  scanf("%d", &repeat);
  int total = repeat*len;
  //ask for dynamic memory
  char* output = (char*)malloc(sizeof(char)*total);
  if(output == NULL) {
    printf("Cannot allocate new string. Exiting...\n");
    exit(0);
  }

  for (int i = 0; i < repeat; i++){
      strcat(output,input);
  }
  printf("%s\n", output);
  free(output);
  return 0;
}