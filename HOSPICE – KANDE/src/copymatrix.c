#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include "sched.h"


typedef matrix Matrix;
struct matrix{
    int nrow;
    int ncol;
    int **data;
};


struct copymatrix_args{
    Matrix *src;
    Matrix *dst;
    int task_index;
    int current_ncol;
};

void create_matrix(Matrix *m,int nrow,int ncol){
    int i;
    n->nrow = nrow;
    n->ncol = ncol;
    m->data = malloc(nrow*sizeof(int*));
    for(i = 0; i < ncol;i++){
        m->data[i] = calloc(ncol,sizeof(int));
    }
}

void init_matrix(Matrix *m){
    int i,j;
    for(i = 0; i < m->nrow;i++){
        for(int j = 0; j < m->ncol;j++){
            m->data[i][j] = rand()%10+1;
        }
    }
}

void free_matrix(Matrix *m){
    int i;
    for(i = 0; i < m->nrow; i++){
        free(m->data[i]);
    }
    free(m);
}

struct copymatrix_args * new_args(Matrix *src,Matrix *dst,int current_ncol){

    struct copymatrix_args *args = malloc(sizeof(struct copymatrix_args));
    
    if(args == NULL)
        return NULL;

    args->src = src;
    args->dst = dst;
    args->task_index = current_ncol-1;
    args->current_ncol = current_ncol;

    return args;
}

void copymatrix_serial(Matrix *src,Matrix *dst){
    for(int i = 0; i < src->nrow; i++){
        for(int j = 0; j < src->ncol;j++){
            dst->data[i][j] = src->data[i][j];
        }
    }
}
void copymatrix(void *closure, struct scheduler *s);

void gentask(struct copymatrix_args *args){
    int rc;
    args->current_ncol += 1;    
    if(args->src->ncol < args->current_ncol)    
        return;
    rc = sched_spawn(copymatrix,new_args(args->src,args->dst,args->current_ncol),s);
    assert(rc >= 0);
}


void copymatrix(void *closure, struct scheduler *s){
    struct copymatrix_args *args = (struct copymatrix_args *)closure;
    int i;
    
    for(i = 0; i < args->src->ncol;i++){
        dst[args->task_index][i] = src[args->task_index][i];
    } 
    free(closure);
    gentask(args);
    gentask(args);
    gentask(args);
}

int main(int argc, char **argv){
    Matrix src,dst;
    
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

    create_matrix(&src,n,m);
    create_matrix(&dst,n,m);
     
    init_matrix(&src);

    clock_gettime(CLOCK_MONOTONIC, &begin);

    if(serial) {
        copymatrix_serial(&src,&src);
    } else {
        rc = sched_init(nthreads, (n + 127) / 128,
                        copymatrix,new_args(&src,&dst,1));
        assert(rc >= 0);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    delay = end.tv_sec + end.tv_nsec / 1000000000.0 -
        (begin.tv_sec + begin.tv_nsec / 1000000000.0);
    
    printf("Done in %lf seconds.\n", delay);
    for(int i = 0; i < src.nrow; i++){
        for(int j = 0; j < src.ncol; j++){
            assert(src.data[i][j] == dst.data[i][j]);
        }
    }
    
    free_matrix(&src);
    free_matrix(&dst);

    return 0;

 usage:
    printf("copymatrix [-n size] [-t threads] [-s]\n");
    return 1;
}
