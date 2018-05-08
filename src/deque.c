#include "deque.h"

deque_t *deque_alloc() {
	deque_t *d = malloc(sizeof(deque_t));
	d->head = d->tail = NULL;
	d->size = 0;
	return d;
}

deque_node_t * deque_node_alloc(void *f, void *c){
	deque_node_t *n = malloc(sizeof(deque_node_t));
	n->f = f;
	n->arg = c;
	return n;
}

void deque_free(deque_t *d) {
	deque_node_t *n = d->head;
	while(n != NULL){
		deque_node_t *del_n = n;
		n = n->next;
		free(del_n);
	}
	free(d);
}

int deque_is_empty(deque_t *d) {
	return d->head == NULL;
}

int deque_size(deque_t *d){
	deque_node_t *n = d->head;
	int cc = 0;
	while(n != d->tail){
		n = n->next;
		cc++;
	}
	return cc;
	// todo size ne fonctionne pas
	//return d->size;
}

void deque_push_head(deque_t *d, deque_node_t *n){
	n->next = d->head;
	n->prev = NULL;
	
	d->head = n;
	if(d->tail == NULL)
		d->tail = n;
	
	d->size++;
}

void deque_push_tail(deque_t *d, deque_node_t *n){
	n->prev = d->tail;
	n->next = NULL;

	d->tail = n;
	if (d->head == NULL) 
		d->head = n;
	
	d->size++;
}

deque_node_t *deque_pop_head(deque_t *d){
	if(d->head != NULL){
		deque_node_t *n = d->head;
		if (d->head == d->tail)
			d->head = d->tail = NULL;
		else
			d->head = n->next;
		d->size--;
		return n;
	}
	return NULL;
}

deque_node_t *deque_pop_tail(deque_t *d){
	if(d->tail != NULL){
		deque_node_t *n = d->tail;
		if (d->head == d->tail)
			d->head = d->tail = NULL;
		else
			d->tail = n->prev;
		d->size--;
		return n;
	}
	return NULL;
}

deque_node_t *deque_peek_front(deque_t *d) {
	return d->head;
}

deque_node_t *deque_peek_back(deque_t *d) {
	return d->tail;
}