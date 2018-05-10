#ifndef	DEQUE_H
#define DEQUE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct deque_node_t {
	struct deque_node_t *next;
	struct deque_node_t *prev;
	void* f;
	void* arg;
} deque_node_t;

typedef struct {
	struct deque_node_t *head;
	struct deque_node_t *tail;
	int size;
} deque_t;


deque_t * deque_alloc();
deque_node_t * deque_node_alloc(void *, void *);

void deque_free(deque_t *d);

int deque_check_integrity(deque_t *d);
int deque_is_empty(deque_t *d);

int deque_size(deque_t *d);

void deque_push_head(deque_t *d, deque_node_t *v);
void deque_push_tail(deque_t *d, deque_node_t *v);

deque_node_t *deque_pop_head(deque_t *d);
deque_node_t *deque_pop_tail(deque_t *d);

deque_node_t *deque_peek_head(deque_t *d);
deque_node_t *deque_peek_tail(deque_t *d);

#endif