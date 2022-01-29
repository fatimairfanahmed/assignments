/*
 * password.c
 * Author: Fatima Irfan
 * Date: 1/26/2022
 * Generate a Bad Password
 */

#include <stdio.h>

//the main function
int main() {
  printf("Enter a word:  ");
  char input[20];
  scanf("%s", input);
  int i = 0;
  while (input[i] != '\0'){
    if (input[i] == 'e'){
      input[i] = '3';
    }
    else if (input[i] == 'l') {
      input[i] = '1';      
    }
    else if (input[i] == 'a') {
      input[i] = '@';      
    }
    i++;
  }
  printf("Your bad password is %s ", input);
  return 0;
}
