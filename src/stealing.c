#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include "sched.h"
#include "deque.h"


struct scheduler{
	Deque **tasks_deque;
	pthread_t *pool;
	pthread_mutex_t mutex;
	int nthreads;
	int cpt_thread;
};

int free_sched(scheduler *s){
	for (int i = 0; i < s->nthreads; ++i)
		free_deque(s->tasks_deque[i]);
	free(s->pool);
	free(s);
	return 0;
}

int initialize(scheduler *s,int nthreads,int qlen){
	s->cpt_thread = 0;
	s->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	s->nthreads = (nthreads == 0) ? sched_default_threads() : nthreads;
	s->pool =(pthread_t*) malloc(sizeof(pthread_t)*nthreads);
	
	s->tasks_deque = malloc(sizeof(*s->tasks_deque) * nthreads);

	for (int i = 0; i < s->nthreads; ++i){
		s->tasks_deque[i] = malloc(sizeof(*s->tasks_deque[i]));
		init(s->tasks_deque[i]);
	}
	
	return 0;
}

int getThreadId(scheduler *s){
	pthread_t t = pthread_self();
	for (int i = 0; i < s->nthreads; ++i)
		if(pthread_equal(t, s->pool[i]))
			return i;
	return -1;
}

int allEmpty(Deque **d, int count) {
	for (int i = 0; i < count; ++i)
		if(!empty(d[i]))
			return 0;
	return 1;
}

Element *stealing(scheduler *s){
	int k = rand() % s->nthreads, i = 0;
	Element *e;
	while(i < s->nthreads || e == NULL){
		Deque *d = s->tasks_deque[(k + i) % s->nthreads];
		if(!empty(d))
			e = pop_tail(d);
		i++;
	}
	return e;
}

void* task_manager(void* ptr){
	scheduler *s = (scheduler*)ptr;
	printf("+ in\n");
	
	int id = getThreadId(s);
	
	do {
		if(empty(s->tasks_deque[id])) {
			pthread_mutex_lock(&s->mutex);
			Element *e = stealing(s);
			pthread_mutex_unlock(&s->mutex);
			
			if(e != NULL){
				((taskfunc) e->f)(e->arg, s);
			}
			else
				sleep(1);
		} else {
			pthread_mutex_lock(&s->mutex);
			Element *e = pop_head(s->tasks_deque[id]);
			pthread_mutex_unlock(&s->mutex);

			if(e != NULL)
				((taskfunc) e->f)(e->arg, s);
		}

		printf("-\t enAttentes: -\t enCours: %d\n", s->cpt_thread);
	} while(0 != s->cpt_thread || !allEmpty(s->tasks_deque, s->nthreads));

	printf("+ out\t enAttentes: -\t enCours: %d\n", s->cpt_thread);
	pthread_exit(NULL);
}

int throws_threads(scheduler *s){
	int i = 0,ret;
	for(i = 0; i<s->nthreads;i++){
		ret = pthread_create(&s->pool[i],NULL,task_manager,s);
		if(ret == -1){
			fprintf(stderr, "%s\n",strerror(ret));
			exit(EXIT_FAILURE);
		}
	}

	for(i = 0; i < s->nthreads;i++){
		pthread_join(s->pool[i],NULL);
	}
	printf("+ join\n");

	return 0;
}

int sched_init(int nthreads, int qlen, taskfunc f, void *closure){
	scheduler *s= malloc(sizeof(*s));
	initialize(s,nthreads,qlen);
	push_head(s->tasks_deque[0],f,closure);
	throws_threads(s);
	free_sched(s);
	srand(time(NULL));
	return 1;
}

int sched_spawn(taskfunc f, void *closure, struct scheduler *s){
	printf("# enter\n");
	int id = getThreadId(s);

	pthread_mutex_lock(&s->mutex);
	push_head(s->tasks_deque[id],f,closure);
	pthread_mutex_unlock(&s->mutex);

	return 1;
}