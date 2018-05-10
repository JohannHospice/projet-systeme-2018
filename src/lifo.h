#ifndef LIFO_H
#define LIFO_H

#include <stdlib.h>
#include <stdio.h>

typedef struct lifo_node {
  void* f;
  void* arg;
  struct lifo_node *next;
} lifo_node_t;

typedef struct {
  lifo_node_t *first;
  int size;
} lifo_t;

lifo_t *lifo_alloc();
void lifo_free(lifo_t *);

void lifo_push(lifo_t *, void *, void *);
lifo_node_t* lifo_pop(lifo_t *);

int lifo_is_empty();

#endif
