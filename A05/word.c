/*
 * Program Name : word.c 
 * CS223 - Spring 2022
 * Author: Fatima Irfan
 * Date: 02/25/2022
 * Takes a word and stores it inside an unisgned int
 */
#include <stdio.h>

int main() {
  char input[4];
  int counter = 0;
  int answer = 0;
  printf("Enter 4 characters: ");
  scanf("%s", input);
  for (int i = 3; i >= 0; i--) {
      unsigned int temp = input[i] << counter;
      printf("Debug: %c = %08X\n", input[i], temp);
      counter = counter + 8;
      answer = answer | temp;
  }
  printf("Your number is: %d (0x%08x)\n", answer,answer);
  return 0;
}
