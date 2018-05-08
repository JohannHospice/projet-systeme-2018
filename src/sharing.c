#include "lifo.h"
#include "sched.h"

struct scheduler{
	lifo_t *stack;
	pthread_t *pool;
	pthread_mutex_t mutex;
	int nthreads;
	int cpt_thread;
};

int free_sched(scheduler *s){
	lifo_free(s->stack);
	free(s->pool);
	free(s);
	return 0;
}

int init_sched(scheduler *s,int nthreads,int qlen){
	s->stack = lifo_alloc();
	//s->stack->qlen = qlen;
	s->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	s->nthreads = (nthreads == 0)? sched_default_threads():nthreads;
	s->pool =(pthread_t*) malloc(sizeof(pthread_t)*nthreads);
	s->cpt_thread = 0;
	
	return 0;
}

void* task_manager(void* ptr){
	scheduler *s = (scheduler*)ptr;
	// printf("+ in\n");

	// si aucune taches en cour dexecution et aucune taches en attentes
	while(s->cpt_thread != 0 || !lifo_is_empty(s->stack)) {
		pthread_mutex_lock(&s->mutex);
		lifo_node_t* e = lifo_pop(s->stack);
		pthread_mutex_unlock(&s->mutex);

		if(e != NULL) {
			pthread_mutex_lock(&s->mutex);
			s->cpt_thread++;
			pthread_mutex_unlock(&s->mutex);
			
			// printf("+ execute \n");
			((taskfunc) e->f)(e->arg, s);
			
			pthread_mutex_lock(&s->mutex);
			s->cpt_thread--;
			pthread_mutex_unlock(&s->mutex);

			free(e);
		}

		// printf("-\t enAttentes: %d\t enCours: %d\n", s->stack->size, s->cpt_thread);
	}

	// printf("+ out\t enAttentes: %d\t enCours: %d\n",s->stack->size, s->cpt_thread);

	pthread_exit(NULL);
}

int throws_threads(scheduler *s){
	int i = 0,ret;
	for(i = 0; i<s->nthreads;i++){
		ret = pthread_create(&s->pool[i], NULL, task_manager, s);
		if(ret == -1){
			fprintf(stderr, "%s\n",strerror(ret));
			exit(EXIT_FAILURE);
		}
	}

	for(i = 0; i < s->nthreads;i++)
		pthread_join(s->pool[i], NULL);
	// printf("+ join\n");

	return 0;
}

int sched_init(int nthreads, int qlen, taskfunc f, void *closure){
	if(nthreads <= 0)
		nthreads = sched_default_threads();

	scheduler *s = malloc(sizeof(*s));
	init_sched(s, nthreads, qlen);
	lifo_push(s->stack, f, closure);
	
	throws_threads(s);

	free_sched(s);
	return 1;
}

int sched_spawn(taskfunc f, void *closure, struct scheduler *s){
	// printf("# enter\n");
	pthread_mutex_lock(&s->mutex);
	lifo_push(s->stack, f, closure);
	pthread_mutex_unlock(&s->mutex);

	return 1;
}
