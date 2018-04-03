#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "sched.h"

static pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;

int free_sched(scheduler *s){
	free_stack(s->tasks_stack);
	free(s);
	return 0;
}


int initialize(scheduler *s,int nthreads,int qlen){
	int nbthreads = (nthreads <= 0)? 2:nthreads;
	s->tasks_stack = malloc(sizeof(*s->tasks_stack));
	s->pool = malloc(sizeof(*s->pool));
	s->tasks_stack->size = 0;
	s->pool->qlen = qlen;
	s->pool->nthreads = nbthreads;
	
	return 0;
}

void* task_manager(void* ptr){
	scheduler *s = (scheduler*)ptr;
	if(isEmpty(s->tasks_stack))
		puts("ici on doit attendre (wait)");

	pthread_mutex_lock(&mutex);
	Element* e =pop(s->tasks_stack);
	pthread_mutex_unlock(&mutex);
	((taskfunc)e->f)(e->arg,s);
	free(e);
	pthread_exit(NULL);// à remplacer par un thread disponible 
	                   //pour prendre une autre tâche
}

int throws_threads(scheduler *s){
	int i = 0,len = s->pool->nthreads;
	for(i = 0; i<len;i++){
		pthread_t thread;
		pthread_create(&thread, NULL,task_manager,s);
	}
	//ici contrôler l'ensemble des threads
	/*
	while(threads_vivant && existence des tâches dans la pile){
	
	}
	
	*/
	return 0;
}

int sched_init(int nthreads, int qlen, taskfunc f, void *closure){
	scheduler *s= malloc(sizeof(*s));
	initialize(s,nthreads,qlen);
	throws_threads(s);
	free_sched(s);
	return 1;
}

int sched_spawn(taskfunc f, void *closure, struct scheduler *s){
	pthread_mutex_lock(&mutex);
	push(s->tasks_stack,f,closure);
	pthread_mutex_unlock(&mutex);
	return 1;
}