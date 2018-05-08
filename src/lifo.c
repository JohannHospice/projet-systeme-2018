#include "lifo.h"

lifo_t *lifo_alloc() {
	lifo_t *l = malloc(sizeof(lifo_t *));
	l->size = 0;
	return l;
}

lifo_node_t *lifo_node_alloc(void *f, void* arg) {
	lifo_node_t *n = malloc(sizeof(lifo_node_t *));
	n->f = f;
	n->arg = arg;
	return n;
}

void lifo_free(lifo_t *l) {
	lifo_node_t *n = l->first;
	while(n != NULL) {
		lifo_node_t *del_n = n;
		n = n->next;
		free(del_n);
	}
	free(l);
}

void lifo_push(lifo_t *l, void *f, void* arg) {
	lifo_node_t *n = lifo_node_alloc(f, arg);
	n->next = l->first;

	l->first = n;
	l->size++;
}

lifo_node_t* lifo_pop(lifo_t *l) {
	if(l->first != NULL) {
		lifo_node_t *n = l->first;
		l->first = n->next;
		l->size--;
		return n;
	}
	return NULL;
}

int lifo_is_empty(lifo_t *l) {
	return l->first == NULL;
}
