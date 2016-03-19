#pragma once

// helper structure for matrix
typedef struct
{
	int size;
	float *mat;
} matrix_t;

// API functions defined in matrix.c, main prog in prog2.c
matrix_t *init_mat_arr( const char *filepath );
void destroy_mat_arr( matrix_t *matrix );
matrix_t *multiply_mat( matrix_t *m1, matrix_t *m2 );
void print_mat( const matrix_t *matrix );
void transpose_mat( matrix_t *matrix );