#ifndef	DEQUE_H
#define DEQUE_H
#endif

#include <stdlib.h>
#include <stdio.h>

typedef struct Element Element;
struct Element{
  void* f;
  void* arg;
  Element *next;
  Element *prev;
};

typedef struct Deque Deque;
struct Deque{
  Element *head;
  Element *tail;
  size_t size;
};

int init(Deque *d);

int push_head(Deque *d,void *f,void* arg);
int push_tail(Deque *d,void *f,void* arg);

Element *pop_head(Deque *d);
Element *pop_tail(Deque *d);

int free_deque(Deque *d);
/*

int insert(Deque *d,void *f,void* arg,int index);
int display(Deque *d);
Element *remove_index(Deque *d, int index);
*/