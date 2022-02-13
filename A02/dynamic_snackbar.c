/*
 * Program Name : dynamic_snackbar.c
 * Author: Fatima Irfan
 * Date: 2/4/2022
 * Make a Dynamic SnackBar from the userinput
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//defining a snack struct
struct snack {
  char name[32];
  float cost;
  int quantity;
};

int main() {
  int number = 0;
  printf("Enter a number of snacks:\n");
  scanf("%d",&number);
  //declaring a struct pointer that points towards the dynamic memory
  struct snack* arraySnacks = malloc(sizeof(struct snack)*number); 
  if(arraySnacks == NULL) {
    printf("Cannot allocate new string. Exiting...\n");
    exit(0);
  }
  for (int i = 0; i < number; i++){
    char name[32];
    float cost = 0.0;
    int quantity = 0;
    printf("Enter a name: \n");
    scanf("%s", name);
    strcpy((arraySnacks + i)->name, name);
    printf("Enter a cost: \n");
    scanf("%f",&cost);
    (arraySnacks+i)->cost = cost;
    printf("Enter a quanitity: \n");
    scanf("%d",&quantity);
    (arraySnacks+i)->quantity = quantity;
  }
  
  printf("\n");
  printf("Welcome to Dynamic Fatima's Snack Bar.\n");
  printf("\n");
  for (int i = 0; i < number; i++){
    printf("%d)  ", i);
    printf("%s      ", arraySnacks[i].name);
    printf("cost: $%.2f    ", arraySnacks[i].cost );
    printf("quanity: %d", arraySnacks[i].quantity);
    printf("\n");
  }
  free(arraySnacks);
  return 0;
}
