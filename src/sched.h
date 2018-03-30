#include <unistd.h>
#include "lifo.h"
#include "thread_pool.h"

struct scheduler;

typedef void (*taskfunc)(void*, struct scheduler *);


typedef struct scheduler scheduler;
struct scheduler{
	Stack *tasks_stack;
	ThreadPool* pool;
};

static inline int
sched_default_threads()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

int sched_init(int nthreads, int qlen, taskfunc f, void *closure);
int sched_spawn(taskfunc f, void *closure, struct scheduler *s);
