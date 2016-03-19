#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

const int mat_count = 2;

// API function to fill matrix
matrix_t *alloc_mat(int size, FILE *hfile )
{
	matrix_t *matrix = malloc( sizeof( matrix_t ) );
	matrix->size = size;
	matrix->mat = malloc( size * size * sizeof( float ) );
	
	if( hfile != NULL )
		for( int it = 0; it < size * size; ++it )
			fscanf( hfile, "%f", matrix->mat + it );
	return matrix;
}

// API function to destroy resources and prevent memory leaks
void destroy_mat( matrix_t *matrix )
{
	free( matrix->mat );
	free( matrix );
}

// API function to display contents of matrix
void print_mat( const matrix_t *matrix )
{
	printf( "Matrix size: %i\nMatrix contents:\n", matrix->size );
	for( int i = 0; i < matrix->size; ++i )
	{
		// neat spacing in printing
		for( int j = 0; j < matrix->size; ++j )
			printf( matrix->mat[ i * matrix->size + j ] < 0.f ? "%.1f\t" : " %.1f\t", matrix->mat[ i * matrix->size + j ] );
		printf( "\n" );
	}
}

// API function to set all matrix elements to zero
void zero_mat( matrix_t *matrix )
{
	memset( matrix->mat, 0, matrix->size * matrix->size * sizeof( float ) );
}

// API function to access a matrix element
float *element_mat( matrix_t *matrix, int i, int j )
{
	if( i < matrix->size && j < matrix->size )
		return matrix->mat + i * matrix->size + j;
	return NULL;
}

// API function to transpose a matrix
void transpose_mat( matrix_t *matrix )
{
	// construct a temporary matrix
	matrix_t* tmp = alloc_mat( matrix->size, NULL );

	// transpose on tmp
	for( int i = 0; i < matrix->size; ++i )
		for( int j = 0; j < matrix->size; ++j )
			*element_mat( tmp, j, i ) = *element_mat( matrix, i, j );

	// destroy the temporary after buffer swap
	float *destroying_buff = matrix->mat;
	matrix->mat = tmp->mat;
	tmp->mat = destroying_buff;
	destroy_mat( tmp );
}

// API function to multiply two matrices
void multiply_mat( matrix_t *dest, matrix_t *m1, matrix_t *m2 )
{
	if( !( dest->size == m1->size && m1->size == m2->size ) )
		return;

	int size = dest->size;
	zero_mat( dest );

	for( int i = 0; i < size; ++i )
		for( int j = 0; j < size; ++j )
			for( int k = 0; k < size; ++k )
				*element_mat( dest, i, j ) += *element_mat( m1, i, k ) * *element_mat( m2, k, j );

}

// API function to get the (i,j)th submatrix from src
void get_sub_mat( matrix_t *dest, matrix_t *src, int i, int j )
{
	for( int k = 0; k < dest->size * dest->size; ++k )
	{
		int ii = k / dest->size;
		int jj = k % dest->size;
		*element_mat( dest, ii, jj ) = *element_mat( src, dest->size * i + ii, dest->size * j + jj );
	}
}

// API function to set the (i,j)th submatrix to dest
void set_sub_mat( matrix_t *dest, matrix_t *src, int i, int j )
{
	for( int k = 0; k < dest->size * dest->size; ++k )
	{
		int ii = k / src->size;
		int jj = k % src->size;
		*element_mat( dest, src->size * i + ii, src->size * j + jj ) = *element_mat( src, ii, jj );
	}
}