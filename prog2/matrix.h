#pragma once

// helper structure for matrix
typedef struct
{
	int size;
	float *mat;
} matrix_t;

// API functions defined in matrix.c, main prog in prog2.c
matrix_t *alloc_mat( int size, FILE *hfile );
void destroy_mat( matrix_t *matrix );
void print_mat( const matrix_t *matrix );
void transpose_mat( matrix_t *matrix );
void multiply_mat( matrix_t *dest, matrix_t *m1, matrix_t *m2 );