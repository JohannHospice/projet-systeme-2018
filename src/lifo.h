#ifndef LIFO_H
#define LIFO_H
#endif

#include <stdlib.h>
#include <stdio.h>


typedef struct Element Element;
struct Element{
  void* f;
  void* arg;
  Element *next;
};
typedef struct Stack Stack;
struct Stack{
  Element *first;
  int size;
  int qlen;
};


int free_stack(Stack *stack);
void push(Stack *stack,void *f,void* arg);
Element* pop(Stack *stack);
int isEmpty();
