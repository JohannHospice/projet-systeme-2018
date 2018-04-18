#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "lifo.h"


struct scheduler;

typedef void (*taskfunc)(void*, struct scheduler *);


typedef struct scheduler scheduler;
struct scheduler{
	Stack *tasks_stack;
	pthread_t *pool;
	pthread_mutex_t stack_mutex;
	pthread_cond_t cond_thread;
	int nthreads;
	int cpt_thread;
};

static inline int
sched_default_threads()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

int sched_init(int nthreads, int qlen, taskfunc f, void *closure);
int sched_spawn(taskfunc f, void *closure, struct scheduler *s);
