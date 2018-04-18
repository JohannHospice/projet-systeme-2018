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

void push(Stack *stack,void *f,void* arg){
  //if(stack->size < stack->qlen){
    Element *new_element = malloc(sizeof(*new_element));
    if(stack == NULL || new_element == NULL)
      exit(EXIT_FAILURE);

    new_element->f = f;
    new_element->arg = arg;
    new_element->next = stack->first;
    stack->first = new_element;
    stack->size += 1;
  //}else{
  //  exit(EXIT_FAILURE);
  //}
}

Element* pop(Stack *stack){
  if(stack == NULL)
    exit(EXIT_FAILURE);

  Element *element = stack->first;

  if(stack != NULL && stack->first != NULL){
    stack->first = element->next;
  }
  stack->size -= 1;
  return element;
}

int isEmpty(Stack *stack){
  return(stack->size == 0);
}
