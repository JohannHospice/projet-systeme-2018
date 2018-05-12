#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include "sched.h"


struct copymatrix_args{
    int **m1;
    int **m2;
    int **mres;
    int n;
    int m;
    int p;
};
struct multmatrix_args * new_args(int **m1,int **m2,int **mres,int n,
                                  int m,int p){

    struct multmatrix_args *args = malloc(sizeof(struct multmatrix_args));
    
    if(args == NULL)
        return NULL;

    args->m1 = m1;
    args->m2 = m2;
    args->mres = mres;
    args->n = n;
    args->m = m;
    args->p = p;

    return args;
}
void initmatrix(int **m,int n,int p){
    for(int i = 0; i < n; i++){
        m[i] = malloc(n*sizeof(*(m[i])));
        
        if(m1 == NULL || m2 == NULL)
            exit(0);
        
        for(int j = 0; j < p;j++){
            m[i][j] = rand()%10+1;
        }

    }
}

void freematrix(int **m,int n){
    for(int i = 0; i < n; i++){
        free(m[i]);
    }
    free(m);
}

void multmatrix_serial(int **m1,int **m2,int **mres,int m,int n,int p){
    for(int i = 0; i < n ;i++){
        for(int j =0; j < p;j++){
            mres[i][j] = 0;
            for(int k =0; k <m;k++){
                mres[i][j] += m1[i][k]*m2[k][j];
            }
        }
    }
}

void multmatrix(void *closure, struct scheduler *s){
    //struct copymatrix_args *args = (struct copymatrix_args *)closure;
    /*int **src = args->src;
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
        return;*/

    rc = sched_spawn(copymatrix,new_args(m1,m2,mres,n,m,p),s);
    assert(rc >= 0);
}


int main(int argc, char **argv){
    int **m1;//m1[n][m] -> tableau à multiplier
    int **m2;//m2[m][n] -> tableau à multiplier
    int **mres;//mres[n][p] -> tableau resultat
    
    struct timespec begin, end;
    double delay;
    int rc;
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

    m1 = malloc(n * sizeof(*m1));
    m2 = malloc(m * sizeof(*m2));
    mres = malloc(n * sizeof(*mres));
    
    if(m1 == NULL || m2 == NULL)
        exit(0);
    
    initmatrix(m1,n,m);
    initmatrix(m2,m,p);
    initmatrix(mres,n,0);

    clock_gettime(CLOCK_MONOTONIC, &begin);

    if(serial) {
        multmatrix_serial(m1,m2,mres,m,n,p);
    }else{
        rc = sched_init(nthreads, (n + 127) / 128,
        multmatrix,new_args(m1,m2,mres,n,m,p));
        assert(rc >= 0);
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
    freematrix(m1,n);
    freematrix(m2,m);
    freematrix(mres,n);
    return 0;

 usage:
    printf("copymatrix [-n size] [-t threads] [-s]\n");
    return 1;
}
