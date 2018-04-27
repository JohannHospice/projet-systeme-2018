#include <unistd.h>
#include <pthread.h>
#include "sched.h"
#include "deque.h"

int sched_init(int nthreads, int qlen, taskfunc f, void *closure){
	// toute deques vide sauf une contenant la tache init
	return -1;
}

int sched_spawn(taskfunc f, void *closure, struct scheduler *s){
	return -1;
}