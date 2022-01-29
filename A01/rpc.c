/*
 * rpc.c
 * Author: Fatima Irfan
 * Date: 1/26/2022
 * Rock Paper Scissor Game
 */

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

//prints AI's choice to the user
void convertWord(int number) {
  if (number == 0){
    printf("AI chose rock\n");
  }
  else if (number == 1){
    printf("AI chose scissors\n");
  }
  else if (number == 2){
    printf("AI chose paper\n");
  }
}

//determines winners based on the choice and updates the scores
int determineWinner(int playerChoice, int* pScore, int* aScore) {
  int aiChoose = rand()%3;
  convertWord(aiChoose);
  if (aiChoose == playerChoice) {
    printf("It is a tie\n");
  }
  else if (aiChoose == 2 && playerChoice == 0){
    *aScore = *aScore + 1;
  }
  else if (playerChoice == 2 && aiChoose == 0) {
    *pScore = *pScore + 1;
  }
  else {
      if (aiChoose < playerChoice){
        *aScore = *aScore + 1;
      }
      else if (playerChoice < aiChoose) {
        *pScore = *pScore + 1;
      }
  }
printf("AI score: %d, Player score: %d\n", *aScore, *pScore); 
}

//converts the user's choice to numbers 
int convertChoiceToNum(char* player) {
  if (player[0] == 'r'){
    return 0;
  }
  else if (player[0] == 's'){
    return 1;
  }
  else if (player[0] == 'p'){
    return 2;
  }
}

//the main function
int main() {
  srand(time(NULL));
  int rounds = 0;
  int i;
  int playerScore = 0;
  int aiScore = 0;

  printf("Welcome to Rock, Paper, Scissors!\nHow many rounds do you want to play?\n");
  scanf("%d", &rounds);
  for (int i = 0; i < rounds; i++){
    char choice[10];
    printf("Which do you choose? rock, paper, or scissors? ");
    scanf("%s", choice);
    int choiceNum = convertChoiceToNum(choice);
    determineWinner(choiceNum, &playerScore, &aiScore);
  }
  if (rounds == 0){
    printf("AI score: %d, Player score: %d\n", aiScore, playerScore);
  }

  if (playerScore > aiScore){
    printf("Player wins!\n");
  }
  else if (playerScore == aiScore){
    printf("It is a tie\n");
  }
  else {
    printf("AI wins\n");
  }
  return 0;
}