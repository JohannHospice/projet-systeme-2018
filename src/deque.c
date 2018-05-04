#include "deque.h"


int init(Deque *d){
	if(d != NULL){
		d->head = NULL;
		d->tail = NULL;
		d->size = 0;
	}
	return 0;
}

int empty(Deque*d){
	if(d->head == NULL)
		return 1;
	return 0;
}


int push_head(Deque *d,void *f,void* arg){
	Element * e = malloc(sizeof(*e));
	//if(e != NULL){
	e->f = f;
	e->arg = arg;
	
	if(d->tail == NULL){
		e->prev = NULL;
		d->head = e;
		d->tail = e;
	}else{
		d->tail->next = e;
		e->prev = d->tail;
		d->tail = e;
	}

		d->size++; 		 
	//}
	return 0;
}

int push_tail(Deque *d,void *f,void* arg){
	Element * e = malloc(sizeof(*e));
	if(e != NULL){
		e->f = f;
		e->arg = arg;
		if(d->tail == NULL){
			e->next = NULL;
			d->head = e;
			d->tail = e;
		}else{
			d->head->prev = e;
			e->next = d->head;
			d->head = e;
		}
		d->size++;
	}
	return 0;
}

Element *pop_tail(Deque *d){
	Element *del_t = NULL;
	if(d != NULL && d->tail != NULL){
		del_t = d->tail;
		d->tail = d->tail->prev;
		d->tail->next = NULL;
		d->size--;
	}
	return del_t;
}

Element *pop_head(Deque *d){
	Element *del_h = NULL;
	if(d != NULL && d->head != NULL){
		del_h = d->head;
		d->head = d->head->next;
		d->head->prev = NULL;
		d->size--;
	}
	//free(del_h);
	return del_h;
}
/*int insert(Deque *d,void *f,void* arg,int index){
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
}*/
int display(Deque *d){
    if (d != NULL){
    	Element * e = d->head;
        while (e != NULL){
        	puts("******************");
        	e = e->next;
        }

    }
    return 0;
}
/*Element *remove_index(Deque *d, int index){
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
}*/

int free_deque(Deque *d){
	if(d != NULL){
		Element *e = d->head;
		while(e != NULL){
			Element *del_e = e;
			e = e->next;
			free(del_e);
		}
		free(e);
		e = NULL;
	}
	free(d);
	return 0;
}

/*
int main(int argc, char const *argv[])
{
	
	Deque *q = malloc(sizeof(*q));

	init(q);
	append(q,1,1);
	append(q,1,1);
	append(q,1,1);
	Element *e = pop_tail(q);
	free(e);
	//remove_head(q);
	display(q);
	free_deque(q);
	return 0;
}

*/