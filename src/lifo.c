#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lifo.h"



int free_stack(Stack *stack){
  if(stack == NULL)
    exit(EXIT_FAILURE);
  Element *courant = stack->first;
  Element *tmp;
  while(courant != NULL){
    tmp = courant;
    courant = courant->next;
    if(courant != NULL)
      free(tmp);
  }
  free(stack);
  return 1;
}
void push(Stack *stack,void *f,void* args){
  Element *new_element = malloc(sizeof(*new_element));
  if(stack == NULL || new_element == NULL)
    exit(EXIT_FAILURE);

  new_element->f = f;
  new_element->args = args;
  new_element->next = stack->first;
  stack->first = new_element;
}

int pop(Stack *stack){
  int res = -1;
  if(stack == NULL)
    exit(EXIT_FAILURE);

  Element *element = stack->first;

  if(stack != NULL && stack->first != NULL){
    stack->first = element->next;
    free(element);
    res = 1;
  }
  return res;
}
int display(Stack *stack){
  if(stack == NULL)
    exit(EXIT_FAILURE);
  Element *courant = stack->first;
  while(courant != NULL){
    puts("*******");
    courant = courant->next;
  }
  return 1;
}
