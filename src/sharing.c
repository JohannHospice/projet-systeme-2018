#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "sched.h"
//#include <pthread.h>



int initialize(scheduler *s,int nthreads,int qlen){
	s->tasks_stack = malloc(sizeof(*s->tasks_stack));
	s->pool = malloc(sizeof(*s->pool));
	s->tasks_stack->size = 0;
	s->pool->nthreads = nthreads;
	s->pool->qlen = qlen;
	return 0;
}

int free_sched(scheduler *s){
	free_stack(s->tasks_stack);
	free(s);
	return 0;
}

int sched_init(int nthreads, int qlen, taskfunc f, void *closure){
	scheduler *s= malloc(sizeof(*s));
	initialize(s,nthreads,qlen);
	push(s->tasks_stack,f,closure);//test ajout
	push(s->tasks_stack,f,closure);//test ajout
	push(s->tasks_stack,f,closure);//test ajout
	push(s->tasks_stack,f,closure);//test ajout
	display(s->tasks_stack);//test d'affichage

	/*if (nthreads <= 0){
		nthreads = sched_default_threads();
	}

	struct scheduler *s = malloc(sizeof(struct scheduler));
	// init lifo shed
	// push f -> lifo

	for(int i = 0; i< nthreads; i++){
		pthread_t thread;
		pthread_create(&thread, NULL, other, closure);
	}*/
	free_sched(s);
	return 1;
}

int sched_spawn(taskfunc f, void *closure, struct scheduler *s){
	//push
	//push(s->lifo, f, closure)
	return 1;
}

void *other(void* f/*, struct scheduler *s*/){
	//args = (scheduler) ;//
	// TODO
	// while true
	// verify lifo empty
	// declare mutex
	// si empty sleep
	// sinon
	// pop
	// (*element retourné)(args element retourné,shedular)
	return f;
}
