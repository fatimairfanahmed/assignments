/*
 * Program Name : sorted_snackbar.c
 * CS223 - Spring 2022
 * Author: Fatima Irfan
 * Date: 02/10/2022
 * Identify whether a matrix is a magic square
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  int quantity;
  float cost;
  struct snack* next;
};

struct snack* insert_sorted(struct snack* snacks, const char* name, int quantity, float cost) {
  struct snack* crrSnack = malloc(sizeof(struct snack)); 
  if(crrSnack == NULL) {
    printf("Cannot allocate new memory. Exiting...\n");
    exit(0);
  }
  if (snacks == NULL){
    strcpy(crrSnack->name, name);
    crrSnack->cost = cost;
    crrSnack->quantity = quantity;
    crrSnack->next = NULL;
    return crrSnack;
  } 
  else {
    strcpy(crrSnack->name, name);
    crrSnack->cost = cost;
    crrSnack->quantity = quantity;
    crrSnack->next = NULL;
  }
  struct snack* head = snacks;
  struct snack* temp = snacks;
  int last = 0;
  int counter = 0;
  while (snacks != NULL){   //I have to deal with 4 cases, crr on the third case 
    if (strcmp(snacks->name, crrSnack->name) > 0){
      if (counter == 0){
        crrSnack->next = head;
        head = crrSnack;
        return head;
      }
      crrSnack->next = snacks;
      temp->next = crrSnack;
      last = 0;
      return head;
    }
    else {
      temp = snacks;
      snacks = snacks->next;
      last = 1;
      counter++;
    }
  }
  if (last == 1){
    temp->next = crrSnack;
    crrSnack->next = NULL;
  }
  return head;
}

void clear(struct snack* snacks) {
  struct snack* head = snacks;
  struct snack* temp = snacks;
  while (temp != NULL) {
    temp = temp->next;
    free(head);
    head = temp;
  }
}

int main() {
  int number = 0;
  printf("Enter a number of snacks:\n");
  scanf("%d",&number);
  struct snack* list =  NULL;
  struct snack* head =  NULL;
  for (int i = 0; i < number; i++){
    char name[32];
    float cost = 0.0;
    int quantity = 0;
    printf("Enter a name: \n");
    scanf("%s", name);
    printf("Enter a cost: \n");
    scanf("%f", &cost);
    printf("Enter a quantity: \n");
    scanf("%d", &quantity);
    list = insert_sorted(list, name, quantity, cost);
  }
  head = list;
  printf("Welcome to Sorted Sally's Snack Bar.\n");
  printf("\n");
  for (int i = 0; i < number; i++){
    printf("%d)  ", i);
    printf("%s      ", list->name);
    printf("cost: $%.2f    ", list->cost );
    printf("quanity: %d", list->quantity);
    printf("\n");
    if (list->next == NULL){
      break;
    } else {
      list = list->next;
    }
  }
  clear(head);
  //free(list);
  return 0;
}

