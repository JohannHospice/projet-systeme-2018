#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include "sched.h"
#include "deque.h"

struct scheduler{
	deque_t **deque;
	pthread_t *pool;
	pthread_mutex_t mutex_deque;
	pthread_mutex_t mutex_cpt;
	int nthreads;
	int cpt;
	int qlen;
};

// scheduler function
void sched_free(scheduler *s){
	for (int i = 0; i < s->nthreads; ++i)
		deque_free(s->deque[i]);
	free(s->deque);
	free(s->pool);
	free(s);
}

scheduler *sched_alloc(int nthreads, int qlen, void *f, void *closure){
	scheduler *s = malloc(sizeof(scheduler));
	s->pool = calloc(nthreads, sizeof(pthread_t));
	s->deque = calloc(nthreads, sizeof(deque_t));
	s->nthreads = nthreads;
	s->qlen = qlen;
	s->cpt = 0;
	s->mutex_deque = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	s->mutex_cpt = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

	for (int i = 0; i < nthreads; ++i)
		s->deque[i] = deque_alloc();

	deque_push_head(s->deque[rand() % nthreads], deque_node_alloc(f, closure));
	
	return s;
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
	
	// printf("+ in\t%d\n", id);

	while(1) {
		
		pthread_mutex_lock(&s->mutex_deque);
		deque_node_t *e = deque_pop_tail(s->deque[id]);
		if(e != NULL) {
			pthread_mutex_unlock(&s->mutex_deque);

			// normal work
			pthread_mutex_lock(&s->mutex_cpt);
			s->cpt++;
			pthread_mutex_unlock(&s->mutex_cpt);

			// printf("- work\t%d\tcpt=%d siz=%d\n", id, s->cpt, deque_size(s->deque[id]));
			((taskfunc) e->f)(e->arg, s);
			free(e);

			pthread_mutex_lock(&s->mutex_cpt);
			s->cpt--;
			pthread_mutex_unlock(&s->mutex_cpt);

		} else {
			pthread_mutex_unlock(&s->mutex_deque);
			// workstealing

			// pick random deque
			int k = rand() % s->nthreads;

			pthread_mutex_lock(&s->mutex_deque);
			for (int i = 0; i < s->nthreads && e == NULL; ++i) 
				e = deque_pop_head(s->deque[(k + i) % s->nthreads]);
			// if one task has been found
			if(e != NULL) {
				pthread_mutex_unlock(&s->mutex_deque);
				
				pthread_mutex_lock(&s->mutex_cpt);
				s->cpt++;
				pthread_mutex_unlock(&s->mutex_cpt);

				// printf("- steal\t%d\n", id);
				((taskfunc) e->f)(e->arg, s);
				free(e);

				pthread_mutex_lock(&s->mutex_cpt);
				s->cpt--;
				pthread_mutex_unlock(&s->mutex_cpt);
			} else {
				pthread_mutex_unlock(&s->mutex_deque);

				pthread_mutex_lock(&s->mutex_cpt);
				// break if all deques empty and all thread asleep
				if(s->cpt == 0) {
					pthread_mutex_unlock(&s->mutex_cpt);
					break;
				}
				pthread_mutex_unlock(&s->mutex_cpt);
				// if not sleep for one millisec
				usleep(1000);
			}

			// printf("id=%d cpt=%d dae=%d\n", id, s->cpt, deque_all_empty);
			
		} 
	}

	pthread_exit(NULL);
}

int sched_init(int nthreads, int qlen, taskfunc f, void *closure){
	// init
	srand(time(NULL));
	
	if(nthreads <= 0)
		nthreads = sched_default_threads();

	scheduler *s = sched_alloc(nthreads, qlen, f, closure);
	
	// run thread
	for(int i = 0; i < s->nthreads;i++){
		int ret = pthread_create(&s->pool[i], NULL, task_manager, s);
		if(ret == -1){
			fprintf(stderr, "%s\n", strerror(ret));
			exit(EXIT_FAILURE);
		}
	}

	// wait for all
	for(int i = 0; i < s->nthreads;i++)
		pthread_join(s->pool[i], NULL);

	// free storage
	sched_free(s);

	return 1;
}

int sched_spawn(taskfunc f, void *closure, struct scheduler *s){
	int id = get_thread_id(s);
	
	// error if deque is full
	if(deque_size(s->deque[id]) >= s->qlen)
		return -1;

	// printf("- spawn\t%d\n", id);

	pthread_mutex_lock(&s->mutex_deque);
	deque_push_tail(s->deque[id], deque_node_alloc(f, closure));
	pthread_mutex_unlock(&s->mutex_deque);

	return 1;
}