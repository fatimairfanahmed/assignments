/*
 * snackbar.c
 * Author: Fatima Irfan
 * Date: 1/26/2022
 * Generate a Bad Password
 */

#include <string.h>
#include <stdio.h>

//a snack struct
struct snack {
   char  name[20];
   double cost;
   int quantity;
};

//a main function
int main() {
  int i = 0;
  struct snack Lays; 
  struct snack Cheetos; 
  struct snack Tang;

  strcpy(Lays.name, "Lays Chips");
  Lays.cost = 1.5;
  Lays.quantity = 0;

  strcpy(Cheetos.name, "CheetoPuff");
  Cheetos.cost = 2.5;
  Cheetos.quantity = 5;

  strcpy(Tang.name, "Lemon Tang");
  Tang.cost = 0.5;
  Tang.quantity = 2;

  struct snack food[3];
  food[0] = Lays;
  food[1] = Cheetos;
  food[2] = Tang;
  
  printf("Welcome to Fatima Struct's Snack Bar.\nHow much money do you have? ");
  double money = 0;
  scanf("%lf", &money);

  for(int i = 0; i < 3; i++){
    printf("%d. %s  cost: %0.2f  quantity: %d \n", i, food[i].name, food[i].cost, food[i].quantity);
  }

  printf("What snack would you like to buy [0,1,2]? \n");
  int choice;
  scanf("%d", &choice);

  if (choice == 0){
    if (food[0].quantity == 0 ){
      printf("Sorry, we are out of Lays Chips\n");
    }
    else if (food[0].cost > money){
      printf("You can't afford it!\n");
    }
    else {
      double change =0;
      change = money - food[0].cost;
      printf("You bought Lays Chips\nYou have %0.2f left\n", change);
    }
  }
  else if (choice == 1){
    if (food[1].quantity == 0 ){
      printf("Sorry, we are out of CheetoPuff\n");
    }
    else if (food[1].cost > money){
      printf("You can't afford it!\n");
    }
    else {
      double change =0;
      change = money - food[1].cost;
      printf("You bought CheetoPuff\nYou have %0.2f left\n", change);
    }
  }
  else if (choice == 2){
    if (food[2].quantity == 0 ){
      printf("Sorry, we are out of Tang\n");
    }
    else if (food[2].cost > money){
      printf("You can't afford it!\n");
    }
    else {
      double change = 0.0;
      change = money - food[2].cost;
      printf("You bought Tang\nYou have %0.2f left\n", change);
    }
  }
  return 0;
}
