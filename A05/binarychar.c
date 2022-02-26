/*
 * Program Name : binarychar.c 
 * CS223 - Spring 2022
 * Author: Fatima Irfan
 * Date: 02/25/2022
 * Takes binary number and outputs corresponding char
 */
#include <stdio.h>

int power(int num, int pow) {
  if (pow == 0){
    return 1;
  }
  else if (pow == 1){
    return num;
  }
  for (int i = 1; i < pow; i++){
    num = num*2;
  }
  return num;
}

int main() {
  char binary[8];
  int answer = 0;
  printf("Enter 8 bits: ");
  scanf("%s", binary);
  for (int i = 0; i < 8; i++){
    if (binary[i] == '1'){
      answer = answer + power(2, 7 - i);
    }
  }
  printf("Your char is: %c\n", answer);
  return 0;
}
