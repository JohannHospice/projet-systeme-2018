#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

typedef struct{
    int nrow;
    int ncol;
    int **data;
} Matrix;

void create_matrix(Matrix *m,int nrow,int ncol);
void init_matrix(Matrix *m);
void free_matrix(Matrix *m);

#endif