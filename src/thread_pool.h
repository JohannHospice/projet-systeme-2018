#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#endif

typedef struct ThreadPool ThreadPool;
struct ThreadPool{
  int nthreads;
  int qlen;
};

int init();
