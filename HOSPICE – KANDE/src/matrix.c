#include "matrix.h"

void create_matrix(Matrix *m,int nrow,int ncol){
    int i;
    m->nrow = nrow;
    m->ncol = ncol;
    m->data = malloc(nrow*sizeof(*m->data));
    
    for(i = 0; i < nrow;i++){
        m->data[i] = calloc(ncol,sizeof(*m->data[i]));
    }


}

void init_matrix(Matrix *m){
    srand(time(NULL));
    int i,j;
    for(i = 0; i < m->nrow;i++){
        for(j = 0; j < m->ncol;j++){
            m->data[i][j] = rand()%10+1;
        }
    }
}

void free_matrix(Matrix *m){
    int i;
    for(i = 0; i < m->nrow; i++){
        free(m->data[i]);
    }
    
}
