#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include "sched.h"
#include "deque.h"


struct scheduler{
	deque_t **deque;
	pthread_t *pool;
	pthread_mutex_t mutex;
	int nthreads;
	int cpt;
};

// scheduler function
int sched_free(scheduler *s){
	for (int i = 0; i < s->nthreads; ++i)
		deque_free(s->deque[i]);
	free(s->pool);
	free(s);
	return 1;
}

scheduler *sched_alloc(int nthreads, int qlen, void *f, void *closure){
	scheduler *s = malloc(sizeof(scheduler));
	s->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	s->pool = (pthread_t *) malloc(nthreads * sizeof(pthread_t));
	s->nthreads = nthreads;
	s->cpt = 0;

	s->deque = calloc(nthreads, sizeof(deque_t));
	for (int i = 0; i < s->nthreads; ++i)
		s->deque[i] = deque_alloc();

	deque_push_head(s->deque[0], deque_node_alloc(f, closure));
	
	return s;
}

// deque complementary functions
int deque_all_size(deque_t **deque, int size){
	int cc = 0;
	for (int i = 0; i < size; ++i)
		cc += deque_size(deque[i]);
	return cc;
}

// slow
int deque_all_empty(deque_t **d, int count) {
	for (int i = 0; i < count; ++i)
		if(!deque_is_empty(d[i]))
			return 0;
	return 1;
}

// slow
int get_thread_id(scheduler *s){
	pthread_t t = pthread_self();
	for (int i = 0; i < s->nthreads; ++i)
		if(pthread_equal(t, s->pool[i]))
			return i;
	return -1;
}

void* task_manager(void* ptr) {
	scheduler *s = (scheduler*) ptr;
	
	int id = get_thread_id(s);
	
	// printf("+ in %d\n", id);

	while(!deque_all_empty(s->deque, s->nthreads) || s->cpt != 0) {
		// printf("- %d\t size: %d\t cpt: %d\n", (int) id, deque_all_size(s->deque, s->nthreads), s->cpt);
		
		if(deque_is_empty(s->deque[id])) {
			// workstealing
			int i = 0, k = rand() % s->nthreads;	
			deque_node_t *e = NULL;

			while(i < s->nthreads && e == NULL) {
				deque_t *d = s->deque[(k + i) % s->nthreads];
				i++;
				
				pthread_mutex_lock(&s->mutex);
				e = deque_pop_tail(d);
				pthread_mutex_unlock(&s->mutex);
			}
	
			if(e == NULL)
				sleep(1);
			else {
				// printf("- %d work stealing on %d\n", id, p);
				pthread_mutex_lock(&s->mutex);
				s->cpt++;
				pthread_mutex_unlock(&s->mutex);

				((taskfunc) e->f)(e->arg, s);
				free(e);

				pthread_mutex_lock(&s->mutex);
				s->cpt--;
				pthread_mutex_unlock(&s->mutex);
			}
		} else {
			// search in self queue
			pthread_mutex_lock(&s->mutex);
			s->cpt++;
			deque_node_t *e = deque_pop_head(s->deque[id]);
			pthread_mutex_unlock(&s->mutex);

			if(e != NULL) {
				// printf("- %d work normal\n", id);
				((taskfunc) e->f)(e->arg, s);
				free(e);
			}

			pthread_mutex_lock(&s->mutex);
			s->cpt--;
			pthread_mutex_unlock(&s->mutex);
		}
	}

	// printf("+ out %d\t size: %d\t cpt: %d\n", (int) id, deque_all_size(s->deque, s->nthreads), s->cpt);
	pthread_exit(NULL);
}

int sched_init(int nthreads, int qlen, taskfunc f, void *closure){
	// init
	srand(time(NULL));
	
	if(nthreads <= 0)
		nthreads = sched_default_threads();


	scheduler *s = sched_alloc(nthreads, qlen, f, closure);

	
	// run thread
	for(int i = 0; i<s->nthreads;i++){
		int ret = pthread_create(&s->pool[i], NULL, task_manager, s);
		if(ret == -1){
			// fprintf(stderr, "%s\n", strerror(ret));
			exit(EXIT_FAILURE);
		}
	}

	// wait for all
	for(int i = 0; i < s->nthreads;i++)
		pthread_join(s->pool[i], NULL);

	// printf("+ join\n");
	printf("+ end enAttentes %d\n", deque_all_size(s->deque, s->nthreads));

	// free storage
	sched_free(s);

	return 1;
}

int sched_spawn(taskfunc f, void *closure, struct scheduler *s){
	int id = get_thread_id(s);

	pthread_mutex_lock(&s->mutex);
	deque_push_head(s->deque[id], deque_node_alloc(f, closure));
	pthread_mutex_unlock(&s->mutex);

	return 1;
}