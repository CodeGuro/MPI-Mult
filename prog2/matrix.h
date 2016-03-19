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
void copy_mat( matrix_t *dest, matrix_t *src );
void zero_mat( matrix_t *dest );
void transpose_mat( matrix_t *matrix );
void multiply_mat( matrix_t *dest, matrix_t *m1, matrix_t *m2 );
void get_sub_mat( matrix_t *dest, matrix_t *src, int i, int j );
void set_sub_mat( matrix_t *dest, matrix_t *src, int i, int j );
void add_mat( matrix_t *dest, matrix_t *m1, matrix_t *m2 );
void copy_buff_mat( float *buff, matrix_t *src, int idx );
void copy_mat_buff( float *buff, matrix_t *dest, int idx );