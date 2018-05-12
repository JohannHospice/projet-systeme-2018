#include "deque.h"

deque_t *deque_alloc() {
	deque_t *d = malloc(sizeof(deque_t));
	d->head = d->tail = NULL;
	d->size = 0;
	return d;
}

deque_node_t * deque_node_alloc(void *f, void *c) {
	deque_node_t *n = malloc(sizeof(deque_node_t));
	n->next = n->prev = NULL;
	n->f = f;
	n->arg = c;
	return n;
}

void deque_free(deque_t *d) {
	deque_node_t *n = d->head;
	while(n != NULL) {
		deque_node_t *del_n = n;
		n = n->next;
		free(del_n);
	}
	n = d->tail;
	while(n != NULL) {
		printf("corrupted deque\n");
		deque_node_t *del_n = n;
		n = n->prev;
		free(del_n);
	}
	free(d);
}

int deque_is_empty(deque_t *d) {
	return d->head == NULL;
}

int deque_size(deque_t *d) {
	return d->size;
}

int deque_check_integrity(deque_t *d) {
	int cc_by_head = 0, cc_by_tail = 0;

	for (deque_node_t *n = d->head; n != NULL; n = n->next)
		cc_by_head++;

	for (deque_node_t *n = d->tail; n != NULL; n = n->next)
		cc_by_tail++;

	return cc_by_tail == cc_by_head && cc_by_head == deque_size(d);
}

void deque_push_head(deque_t *d, deque_node_t *n) {
	if(d->head == NULL)
		d->tail = d->head = n;
	else {
		n->next = d->head;
		d->head->prev = n;
		d->head = n;
	}
	
	d->size++;
}

void deque_push_tail(deque_t *d, deque_node_t *n) {
	if (d->tail == NULL)
		d->head = d->tail = n;
    else {
		n->prev = d->tail;
		d->tail->next = n;
		d->tail = n;
    }
	
	d->size++;
}

deque_node_t *deque_pop_head(deque_t *d) {
    if (deque_is_empty(d))
    	return NULL;

    deque_node_t* n = d->head;
    d->head = d->head->next;

    if (d->head == NULL)
        d->tail = NULL;
    else
        d->head->prev = NULL;

	d->size--;
    return n;
}

deque_node_t *deque_pop_tail(deque_t *d) {
    if (deque_is_empty(d))
    	return NULL;

    deque_node_t* n = d->tail;
    d->tail = d->tail->prev;

    if (d->tail == NULL)
        d->head = NULL;
    else
        d->tail->next = NULL;
    d->size--;
    return n;
}

deque_node_t *deque_peek_front(deque_t *d) {
	return d->head;
}

deque_node_t *deque_peek_back(deque_t *d) {
	return d->tail;
}