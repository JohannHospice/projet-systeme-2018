#include "deque.h"


int init(Deque *d){
	if(d != NULL){
		d->head = NULL;
		d->tail = NULL;
		d->size = 0;
	}
	return 0;
}

int append(Deque *d,void *f,void* arg){
	Element * e = malloc(sizeof(*e));
	if(e != NULL){
		e->f = f;
		e->arg = arg;
		if(d->tail == NULL){
			e->prev = NULL;
			d->head = e;
			d->tail = e;
		}else{
			d->tail->next = e;
			d->prev = d->tail;
			d->tail = e;
		}
		d->size++; 		 
	}
	return 0;
}

int prepend(Deque *d,void *f,void* arg){
	Element * e = malloc(sizeof(*e));
	if(e != NULL){
		e->f = f;
		e->arg = arg;
		if(d->tail == NULL){
			d->next = NULL;
			d->head = e;
			d->tail = e;
		}else{
			d->head->prev = e;
			d->next = d->head
			d->head = e;
		}
		d->size++;
	}
	return 0;
}
int insert(Deque *d,void *f,void* arg,int index){
	if(d != NULL){
		int i = 1;
		Element * e_tmp = d->head;
		while(e_tmp != NULL && i <= index){
			if(i == index){
				if(e_tmp->next == NULL){
					d = prepend(d,void *f,void* arg);
				}
			}else{
				e->f = f;
				e->arg = arg;
				e_tmp->next->prev = e;
				e_tmp->prev->next = e;
				e->prev = e_tmp->prev
				e->next = e_tmp;
				d->size++;
			}else{
				e_tmp = e_tmp->next;
			}
			i++;
		}
	}
	return 0;
}
int display(Deque *d){
    if (d != NULL){
    	Element * e = d->head;
        while (p_temp != NULL){
        	puts("******************");
        }

    }
    return 0;
}
Element *remove_index(Deque *d, int index){
	Element * e_tmp = NULL;
    if (d != NULL){
    	e_tmp = d->head;
        int i = 1;
        while(e_tmp != NULL && i <= index){
            if (i == index){
                if(e_tmp->next == NULL){
                   	d->tail = e_tmp->prev;
                    d->tail->next = NULL;
                }else if (e_tmp->prev == NULL){
                    d->head = e_tmp->next;
                    d->head->prev = NULL;
                }else{
                    e_tmp->next->prev = e_tmp->prev;
                    e_tmp->prev->next = e_tmp->next;
                }
                d->size--;
                break;
            }else{
                e_tmp = e_tmp->next;
            }
            i++;
        }
    }   
    return e_tmp;
}

int free_deque(Deque *d){
	if(d != NULL){
		Element *e = d->head;
		while(e != NULL){
			Element *del_e = e;
			e = e->next;
			free(del_e);
		}
	}
	free(e);
	e = NULL;
	return 0;
}