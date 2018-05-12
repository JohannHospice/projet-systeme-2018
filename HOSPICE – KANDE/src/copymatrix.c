#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include "sched.h"


struct copymatrix_args{
    int **src;
    int **dst;
    int n;
    int m;
    int curs;
    int index;
};

struct copymatrix_args * new_args(int **src,int **dst,int n,
                                  int m,int curs){

    struct copymatrix_args *args = malloc(sizeof(struct copymatrix_args));
    
    if(args == NULL)
        return NULL;

    args->src = src;
    args->dst = dst;
    args->n = n;
    args->m = m;
    args->curs = curs;
    args->index = curs-1;

    return args;
}

void copymatrix_serial(int **src,int **dst,int n,int m){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m;j++){
            dst[i][j] = src[i][j];
        }
    }
}

void copymatrix(void *closure, struct scheduler *s){
    struct copymatrix_args *args = (struct copymatrix_args *)closure;
    int **src = args->src;
    int **dst = args->dst;
    int n = args->n;
    int m = args->m;
    int index = args->index;
    int curs = args->curs;
    int rc;
    for(int i = 0; i < m ;i++){
        dst[index][i] = src[index][i];
    }

    free(closure);

    curs += 1;
    
    if(n < curs)
        return;

    rc = sched_spawn(copymatrix,new_args(src,dst,n,m,curs),s);
    assert(rc >= 0);
}

int main(int argc, char **argv){
    int **src;
    int **dst;
    
    struct timespec begin, end;
    double delay;
    int rc;
    int n = 24000,m =10000;
    int nthreads = -1;
    int serial = 0;

    while(1) {
        int opt = getopt(argc, argv, "sn:t:");
        if(opt < 0)
            break;
        switch(opt) {
        case 's':
            serial = 1;
            break;
        case 'n':
            n = atoi(optarg);
            if(n <= 0)
                goto usage;
            break;
        case 'm':
            m = atoi(optarg);
            if(m <= 0)
                goto usage;
            break;
        case 't':
            nthreads = atoi(optarg);
            if(nthreads <= 0)
                goto usage;
            break;
        default:
            goto usage;
        }
    }

    src = malloc(n * sizeof(*src));
    dst = malloc(n * sizeof(*dst));
    
    if(src == NULL || dst == NULL)
        exit(0);
    
    for(int i = 0; i < n; i++){
        src[i] = malloc(m*sizeof(*(src[i])));
        dst[i] =  malloc(m*sizeof(*(dst[i])));
        
        if(src == NULL || dst == NULL)
            exit(0);
        
        for(int j = 0; j < m;j++){
            dst[i][j] = 0;
            if(j % 2 == 0)
                src[i][j] = j;
            else
                src[i][j] = -j;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &begin);

    if(serial) {
        copymatrix_serial(src,dst,n,m);
    } else {
        rc = sched_init(nthreads, (n + 127) / 128,
                        copymatrix,new_args(src,dst,n,m,1));
        assert(rc >= 0);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    delay = end.tv_sec + end.tv_nsec / 1000000000.0 -
        (begin.tv_sec + begin.tv_nsec / 1000000000.0);
    printf("Done in %lf seconds.\n", delay);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            assert(src[i][j] == dst[i][j]);
        }
    }

    for(int i = 0; i < n; i++){
        free(src[i]);
        free(dst[i]);
    }

    free(src);
    free(dst);
    return 0;

 usage:
    printf("copymatrix [-n size] [-t threads] [-s]\n");
    return 1;
}
