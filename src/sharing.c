#include <unistd.h>
#include <pthread.h>
#include "sched.h"

// stack

struct element{
	void* f;
	void* closure;
}
/*
creer stack
*/
struct lifo{
	//tasks
	struct element **e;
	int size;
};

struct scheduler {
	pthread_t **pthreads;
	struct *lifo;
};

int sched_init(int nthreads, int qlen, taskfunc f, void *closure){
	if (nthreads <= 0){
		nthreads = sched_default_threads();
	}

	struct scheduler *s = malloc(sizeof(struct scheduler));
	// init lifo shed
	// push f -> lifo 

	for(int i = 0; i< nthreads; i++){
		pthread_t thread;
		pthread_create(&thread, NULL, other, closure);
	}
	return 1;
}

int sched_spawn(taskfunc f, void *closure, struct scheduler *s){
	//push
	return 1;
}

void *other(void*/*f, struct scheduler *s*/){
	args = (scheduler) ;// 
	// TODO
	// while true
	// verify lifo empty
	// declare mutex
	// si empty sleep
	// sinon
	// (*element retourné)(args element retourné,shedular)
	// pop
	return f;
}

