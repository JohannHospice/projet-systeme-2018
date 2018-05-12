#include "lifo.h"
#include "sched.h"

struct scheduler{
	lifo_t *stack;
	pthread_t *pool;
	pthread_mutex_t mutex_lifo;
	pthread_mutex_t mutex_cpt;

	int nthreads;
	int cpt_thread;
	int qlen;
};

int free_sched(scheduler *s){
	lifo_free(s->stack);
	free(s->pool);
	free(s);
	return 0;
}

scheduler * alloc_sched(int nthreads, int qlen, void* f, void* closure){
	scheduler *s = malloc(sizeof(scheduler));
	
	s->stack = lifo_alloc();
	s->pool = calloc(nthreads, sizeof(pthread_t));
	s->mutex_lifo = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	s->mutex_cpt = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	s->nthreads = nthreads;
	s->qlen = qlen;
	s->cpt_thread = 0;
	
	lifo_push(s->stack, f, closure);

	return s;
}

void* task_manager(void* ptr){
	scheduler *s = (scheduler*) ptr;
	// printf("+ in\n");

	// si aucune taches en cour dexecution et aucune taches en attentes
	while(1) {//s->cpt_thread != 0 || !lifo_is_empty(s->stack)) {
		
		pthread_mutex_lock(&s->mutex_lifo);
		if(lifo_is_empty(s->stack)) {
			pthread_mutex_unlock(&s->mutex_lifo);
			
			pthread_mutex_lock(&s->mutex_cpt);
			if(s->cpt_thread == 0){
				pthread_mutex_unlock(&s->mutex_cpt);
				break;
			}
			pthread_mutex_unlock(&s->mutex_cpt);

		} else {
			lifo_node_t* e = lifo_pop(s->stack);
			pthread_mutex_unlock(&s->mutex_lifo);
	
			pthread_mutex_lock(&s->mutex_cpt);
			s->cpt_thread++;
			pthread_mutex_unlock(&s->mutex_cpt);

			((taskfunc) e->f)(e->arg, s);
			free(e);

			pthread_mutex_lock(&s->mutex_cpt);
			s->cpt_thread--;
			pthread_mutex_unlock(&s->mutex_cpt);
		} 

		// printf("-\t enAttentes: %d\t enCours: %d\n", s->stack->size, s->cpt_thread);
	}

	// printf("+ out\t enAttentes: %d\t enCours: %d\n",s->stack->size, s->cpt_thread);

	pthread_exit(NULL);
}

int sched_init(int nthreads, int qlen, taskfunc f, void *closure){
	if(nthreads <= 0)
		nthreads = sched_default_threads();

	scheduler *s = alloc_sched(nthreads, qlen, f, closure);
	
	int i = 0;
	for(i = 0; i<s->nthreads;i++){
		int ret = pthread_create(&s->pool[i], NULL, task_manager, s);
		if(ret == -1){
			fprintf(stderr, "%s\n",strerror(ret));
			exit(EXIT_FAILURE);
		}
	}

	for(i = 0; i < s->nthreads;i++)
		pthread_join(s->pool[i], NULL);

	free_sched(s);
	return 1;
}

int sched_spawn(taskfunc f, void *closure, struct scheduler *s){
	if(s->stack->size >= s->qlen)
		return -1;

	// printf("# enter\n");
	pthread_mutex_lock(&s->mutex_lifo);
	lifo_push(s->stack, f, closure);
	pthread_mutex_unlock(&s->mutex_lifo);

	return 1;
}
