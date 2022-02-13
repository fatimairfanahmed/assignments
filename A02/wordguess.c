/*
 * Program Name : wordguess.c
 * Author: Fatima Irfan
 * Date: 2/4/2022
 * Ask the user to Guess a Word 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main() {
  srand(time(NULL));
  //initializing a file object
  FILE* infile;
  infile = fopen("words.txt", "r");
  if (infile == NULL){
    printf("Unable to open file\n");
    exit(0);
  }
  char buffer[32]; 
  fgets(buffer,32, infile);
  int total = atoi(buffer);
  int random = rand()%total;

  for (int f = 0; f < random; f++){
      fgets(buffer,32, infile);
  }

  char* word = strtok(buffer, "\n\r");
  printf("Welcome to Word Guess!\n");
  printf("\n");
  int len = strlen(word);
  char* guess = (char*)malloc(sizeof(char)*(len+1)); 
  if(guess == NULL) {
    printf("Cannot allocate new string. Exiting...\n");
    exit(0);
  }
  for (int j = 0; j < len; j++){
    guess[j] = '_';
  }
  guess[len] = '\0';
  int complete = 0;
  int turns = 1;
  while (complete < len){
    char crr;
    printf("Turn: %d\n", turns);
    printf("\n");
    turns++;
    for (int j = 0; j < len; j++) {
      printf("%c",guess[j]);
      printf(" ");
    }
    printf("\n");
    printf("\nGuess a character: ");
    scanf(" %c", &crr);
    int found = 0;
    for (int k = 0; k < len; k++){
      if (word[k] == crr && guess[k] != crr){
        guess[k] = crr;
        complete++;
        found = 1;
      } 
    }
    if (found == 0){
      printf("Sorry, %c not found!\n", crr);
      printf("\n");
    }
  }
  for (int k = 0; k < len; k++) {
    printf("%c",guess[k]);
    printf(" ");
  }
  printf("\n");
  printf("You won in %d turns!\n",turns);
  free(guess);
  fclose(infile);
  return 0;
}
