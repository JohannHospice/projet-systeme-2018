#include <unistd.h>
#include <pthread.h>
#include "sched.h"
#include "deque.h"

int idthread = 0;

struct scheduler{
	Deque **tasks_deques;
	pthread_t *pool;
	pthread_mutex_t deque_mutex;
	pthread_cond_t cond_thread;
	int nthreads;
	int cpt_thread;
	int id; 
};

int free_sched(scheduler *s){
	for (int i = 0; i < s->nthreads; ++i)
		free_deque(s->tasks_deque[i]);
	free(s->pool);
	free(s);
	return 0;
}

int initialize(scheduler *s,int nthreads,int qlen){
	s->tasks_deque = malloc(sizeof(*s->tasks_deque) * nthreads);
	for (int i = 0; i < s->nthreads; ++i)
		init(s->tasks_deque[i]);
	s->deque_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	s->cond_thread = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
	s->nthreads = (nthreads == 0)? sched_default_threads():nthreads;
	s->pool =(pthread_t*) malloc(sizeof(pthread_t)*nthreads);
	s->cpt_thread = 0;
	s->id = 0;
	
	return 0;
}

void* task_manager(void* ptr){
	scheduler *s = (scheduler*)ptr;
	printf("+ in\n");
	
	int id = s->id;
	s->id++;
	
	do {
		printf("-\t enAttentes: %d\t enCours: %d\n", s->tasks_stack->size, s->cpt_thread);
	} while(false);

	printf("+ out\t enAttentes: %d\t enCours: %d\n",s->tasks_stack->size, s->cpt_thread);
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
	// printf("+ join\n");

	return 0;
}

int sched_init(int nthreads, int qlen, taskfunc f, void *closure){
	scheduler *s= malloc(sizeof(*s));
	initialize(s,nthreads,qlen);
	push_head(s->tasks_deque[0],f,closure);
	throws_threads(s);
	free_sched(s);
	return 1;
}

int sched_spawn(taskfunc f, void *closure, struct scheduler *s){
	// printf("# enter\n");
	pthread_mutex_lock(&s->deque_mutex);
	push_head(s->tasks_deque[idthread],f,closure);
	pthread_mutex_unlock(&s->deque_mutex);

	return 1;
}