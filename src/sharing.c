#include "sched.h"

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

int end_cond(scheduler *s){
	if(s->cpt_thread == s->nthreads && isEmpty(s->tasks_stack)){
		puts("test");
		//pthread_cond_signal(&s->cond_thread);
		return 1;		
	}
	return 0;
}

void* task_manager(void* ptr){
	scheduler *s = (scheduler*)ptr;
	
	while(!end_cond(s)){
		pthread_mutex_lock(&s->stack_mutex);
		
		s->cpt_thread +=1;
		if(isEmpty(s->tasks_stack)){
			pthread_cond_wait(&s->cond_thread,&s->stack_mutex);
			//if(s->cpt_thread == s->nthreads && s->tasks_stack->size == 0)
			//	break;
		}
		if(s->tasks_stack->size != 0){
			Element* e =pop(s->tasks_stack);
			s->cpt_thread -=1;
			pthread_mutex_unlock(&s->stack_mutex);
			((taskfunc)e->f)(e->arg,s);
			free(e);	
		}		
	}
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
	pthread_mutex_lock(&s->stack_mutex);
	push(s->tasks_stack,f,closure);
	pthread_cond_signal(&s->cond_thread);
	pthread_mutex_unlock(&s->stack_mutex);

	return 1;
}