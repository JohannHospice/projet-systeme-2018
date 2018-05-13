#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include "matrix.h"
#include "sched.h"




struct multmatrix_args{
    Matrix *src;
    Matrix *dst;
};

struct multmatrix_args * new_args(Matrix *src,Matrix *dst){
    struct multmatrix_args *args = malloc(sizeof(struct multmatrix_args));
    if(args == NULL)
        return NULL;
    args->src = src;
    args->dst = dst;
    return args;
}


void multmatrix_serial(Matrix *m1,Matrix *m2,Matrix *res){
    for(int i = 0; i < m1->nrow ;i++){
        for(int j =0; j < m2->ncol;j++){
            for(int k =0; k <m1->ncol;k++){
                res->data[i][j] += m1->data[i][k]*m2->data[k][j];
            }
        }
    }
}

void multmatrix(void *closure, struct scheduler *s){
    struct multmatrix_args *args = (struct multmatrix_args *)closure;
    int rc;
    //ici mettre le code du parallèlisme
    free(closure);
    rc = sched_spawn(multmatrix,new_args(args->src,args->dst),s);
    assert(rc >= 0);
}


int main(int argc, char **argv){
    Matrix A,B,C;
    
    struct timespec begin, end;
    double delay;
    //int rc;
    int n = 24000,m =10000,p=20000;
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
        case 'p':
            p = atoi(optarg);
            if(p <= 0)
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
    create_matrix(&A,n,m);
    create_matrix(&B,m,p);
    create_matrix(&C,n,p);

    init_matrix(&A);
    init_matrix(&B);

    clock_gettime(CLOCK_MONOTONIC, &begin);

    if(serial){
        multmatrix_serial(&A,&B,&C);
    }else{
        //rc = sched_init(nthreads, (n + 127) / 128,
        //multmatrix,new_args(src,dst));
        //assert(rc >= 0);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    delay = end.tv_sec + end.tv_nsec / 1000000000.0 -
        (begin.tv_sec + begin.tv_nsec / 1000000000.0);
    printf("Done in %lf seconds.\n", delay);

    /*for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            assert(src[i][j] == dst[i][j]);
        }
    }*/
    free_matrix(&A);
    free_matrix(&B);
    free_matrix(&C);
    return 0;

 usage:
    printf("copymatrix [-n size] [-t threads] [-s]\n");
    return 1;
}
