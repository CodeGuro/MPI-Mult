#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

const int mat_count = 2;

// helper function to fill matrix
void alloc_mat( matrix_t *matrix, int size, FILE *hfile )
{
	matrix->size = size;
	matrix->mat = malloc( size * size * sizeof( float ) );
	
	if( hfile != NULL )
		for( int it = 0; it < size * size; ++it )
			fscanf( hfile, "%f", matrix->mat + it );
}

// API function to initialize two matrices from file
matrix_t *init_mat_arr( const char *filepath )
{
	FILE *hfile = fopen( filepath, "r" );

	matrix_t *matrix = malloc( 2 * sizeof( matrix_t ) );

	// fill the matrix size
	char *buff = malloc( 128 * sizeof( char ) );
	int size = atoi( fgets( buff, 128, hfile ) );
	free( buff );

	// fill the two matrices with data
	alloc_mat( matrix + 0, size, hfile );
	alloc_mat( matrix + 1, size, hfile );

	fclose( hfile );

	return matrix;
}

// helper function to destroy resources and prevent memory leaks
void destroy_mat( matrix_t *matrix )
{
	free( matrix->mat );
}

float *get_element( matrix_t *matrix, int i, int j )
{
	if( i < matrix->size && j < matrix->size )
		return matrix->mat + matrix->size * i + j;
	return NULL;
}

// API function to free resources generated from init_mat_arr
void destroy_mat_arr( matrix_t *matrix )
{
	destroy_mat( matrix + 0 );
	destroy_mat( matrix + 1 );
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
	matrix_t tmp;
	alloc_mat( &tmp, matrix->size, NULL );

	// transpose on tmp
	for( int i = 0; i < matrix->size; ++i )
		for( int j = 0; j < matrix->size; ++j )
			*element_mat( &tmp, j, i ) = *element_mat( matrix, i, j );

	// destroy the temporary after buffer swap
	float *destroying_buff = matrix->mat;
	matrix->mat = tmp.mat;
	tmp.mat = destroying_buff;
	destroy_mat( &tmp );
}