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
void print_mat( const matrix_t *matrix );