#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#include "matrix.h"
#include "sched.h"


struct copymatrix_args{
    Matrix *src;
    Matrix *dst;
    int task_index;
    int current_nrow;
};


struct copymatrix_args * new_args(Matrix *src,Matrix *dst,int current_nrow){

    struct copymatrix_args *args = malloc(sizeof(struct copymatrix_args));
    
    if(args == NULL)
        return NULL;

    args->src = src;
    args->dst = dst;
    args->task_index = current_nrow-1;
    args->current_nrow = current_nrow;

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

void gentask(Matrix *src,Matrix *dst,int *current_nrow,int nrow,scheduler *s){
    int rc;
    *current_nrow += 1;    
    if(nrow < *current_nrow)    
        return;
    rc = sched_spawn(copymatrix,new_args(src,dst,*current_nrow),s);
    assert(rc >= 0);
}


void copymatrix(void *closure, struct scheduler *s){
    struct copymatrix_args *args = (struct copymatrix_args *)closure;
    Matrix *src = args->src;
    Matrix *dst = args->dst;
    int current_nrow = args->current_nrow;
    int task_index= args->task_index ;
    int nrow = src->nrow;
    int i;
    for(i = 0; i < args->src->ncol;i++){
        dst->data[task_index][i] = src->data[task_index][i];
    }
    free(closure);
    gentask(src,dst,&current_nrow,nrow,s);
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
        copymatrix_serial(&src,&dst);
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
