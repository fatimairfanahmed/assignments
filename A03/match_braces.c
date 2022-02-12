/*
 * Program Name : match_braces.c 
 * CS223 - Spring 2022
 * Author: Fatima Irfan
 * Date: 02/10/2022
 * Identify matched braces from a given file
 */

#include <stdio.h>
#include <stdlib.h>

struct node {
  char sym;
  int linenum;
  int colnum;
  struct node* next;
};

// Push a new node to a stack (implemented as a linked list). 
// The new node should store the given symbol, line number, and column number
// Param sym: a character symbol, '{' for this program
// Param line: the line number of the symbol
// Param line: the column number of the symbol
// Param top: the top node of the stack (NULL if empty)
// Returns the new top of the stack
struct node* push(char sym, int line, int col, struct node* top) {
  struct node* crrNode = malloc(sizeof(struct node));
  if (crrNode == NULL) {
    printf("Cannot allocate new memory. Exiting...\n");
    exit(0);
  }
  crrNode->sym = sym;
  crrNode->linenum = line;
  crrNode->colnum = col;
  crrNode->next = top;
  top = crrNode;
  return top;
}

// Pop the top node from a stack (implemented as a linked list) and frees it
// Param top: the top node of the current stack (NULL if empty)
// Returns the new top of the stack
struct node* pop(struct node* top) {
  struct node* temp = top;
  top = top->next;
  temp->next = NULL;
  temp = NULL;
  return top;
}

// Delete (e.g. free) all nodes in the given stack
// Param top: the top node of the stack (NULL if empty)
void clear(struct node* top) {
  if (top == NULL){
    free(top);
    return;
  }
  struct node* temp = top;
  while (top != NULL){
    temp = top;
    top = top->next;
    temp->next = NULL;
    free(temp);
  }
}

// Print all nodes in the given stack (from top to bottom)
// Param top: the top node of the stack (NULL if empty)
void print(struct node* top) {
  if (top == NULL){
    printf("NULL");
    return;
  }
  int counter = 1;
  struct node* temp = top;
  while(temp != NULL){
    printf("%d)  ", counter);
    printf("%c  ", top->sym);
    printf("%d  ", top->colnum);
    printf("%d  ", top->linenum);
    temp = temp->next;
  }
  temp = NULL;
}

int main(int argc, char* argv[]) {
  struct node* stack = NULL;
  int line = 0;
  int column = 0;
  FILE* infile;
  infile = fopen("prog2.c", "r");
  char curr = '?';
  while (curr != EOF){
    curr = fgetc(infile);
    if (curr == '{'){
      stack = push(curr, line, column, stack);
    }
    else if (curr == '}'){
      if (stack == NULL){
        printf("Unmatched brace on Line %d and Column %d", stack->linenum, stack->colnum);
      }
      else {
        printf("Found matching braces:(%d, %d) -> (%d, %d)", stack->linenum,stack->colnum, line,column );
      }
      stack = pop(stack);
    }
    column = column + 1;
    if (curr == '\n'){
      line = line + 1;
      column = 0;
    }
  }
  while ( stack != NULL){
    printf("Unmatched brace on Line %d and Column %d", stack->linenum, stack->colnum);
    stack = pop(stack);
  }
  pclose(infile);
  clear(stack);
  free(stack);
  stack == NULL;
  return 0;
}
