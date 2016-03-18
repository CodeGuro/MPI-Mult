#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

const int mat_count = 2;

// helper function to fill matrix
void alloc_mat( matrix_t *matrix, int size, FILE *hfile )
{
	matrix->size = size;
	matrix->mat = malloc( matrix->size * sizeof( float* ) );
	for( int it = 0; it < matrix->size; ++it )
	{
		matrix->mat[ it ] = malloc( matrix->size * sizeof( float ) );

		for( int it2 = 0; it2 < matrix->size; ++it2 )
			fscanf( hfile, "%f", matrix->mat[ it ] + it2 );
	}
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
	for( int it = 0; it < matrix->size; ++it )
		free( matrix->mat[ it ] );

	free( matrix->mat );
}

// API function to destroy the matrix generated from 1
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
			printf( matrix->mat[ i ][ j ] < 0.f ? "%.1f\t" : " %.1f\t", matrix->mat[ i ][ j ] );
		printf( "\n" );
	}
}