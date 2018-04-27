#include "lifo.h"
#include "sched.h"

struct scheduler{
	Stack *tasks_stack;
	pthread_t *pool;
	pthread_mutex_t stack_mutex;
	pthread_cond_t cond_thread;
	int nthreads;
	int cpt_thread;
};

int free_sched(scheduler *s){
	free_stack(s->tasks_stack);
	free(s->pool);
	free(s);
	return 0;
}

int initialize(scheduler *s,int nthreads,int qlen){
	
	s->tasks_stack = malloc(sizeof(*s->tasks_stack));
	s->tasks_stack->size = 0;
	s->tasks_stack->qlen = qlen;
	s->stack_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	s->cond_thread = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
	s->nthreads = (nthreads == 0)? sched_default_threads():nthreads;
	s->pool =(pthread_t*) malloc(sizeof(pthread_t)*nthreads);
	s->cpt_thread = 0;
	
	return 0;
}

void* task_manager(void* ptr){
	scheduler *s = (scheduler*)ptr;
	// printf("+ in\n");
	
	do {
		pthread_mutex_lock(&s->stack_mutex);
		Element* e = pop(s->tasks_stack);
		pthread_mutex_unlock(&s->stack_mutex);

		if(e != NULL) {
			pthread_mutex_lock(&s->stack_mutex);
			s->cpt_thread++;
			pthread_mutex_unlock(&s->stack_mutex);
			
			// printf("+ execute \n");
			((taskfunc) e->f)(e->arg, s);
			
			pthread_mutex_lock(&s->stack_mutex);
			s->cpt_thread--;
			pthread_mutex_unlock(&s->stack_mutex);

			free(e);
		}

		// printf("-\t enAttentes: %d\t enCours: %d\n", s->tasks_stack->size, s->cpt_thread);
	} while(0 != s->cpt_thread || !isEmpty(s->tasks_stack)); // si aucune taches en cour dexecution et aucune taches en attentes

	// printf("+ out\t enAttentes: %d\t enCours: %d\n",s->tasks_stack->size, s->cpt_thread);

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
	push(s->tasks_stack,f,closure);
	throws_threads(s);
	free_sched(s);
	return 1;
}

int sched_spawn(taskfunc f, void *closure, struct scheduler *s){
	// printf("# enter\n");
	pthread_mutex_lock(&s->stack_mutex);
	push(s->tasks_stack,f,closure);
	pthread_mutex_unlock(&s->stack_mutex);

	return 1;
}