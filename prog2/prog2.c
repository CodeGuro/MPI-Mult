#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// helper structure for matrix
typedef struct
{
	int size;
	float **mat;
} matrix_t;

// helper function to initialize and fill matrix from file
void init_mat( matrix_t *matrix, const char *filepath )
{
	FILE *hfile = fopen( filepath, "r" );

	// fill the matrix size
	char *buff = malloc( 128 * sizeof( char ) );
	matrix->size = atoi( fgets( buff, 128, hfile ) );
	free( buff );

	// fill the matrix data
	matrix->mat = malloc( matrix->size * sizeof( float* ) );
	for( int it = 0; it < matrix->size; ++it )
	{
		matrix->mat[ it ] = malloc( matrix->size * sizeof( float ) );
		
		for( int it2 = 0; it2 < matrix->size; ++it2 )
			fscanf( hfile, "%f", matrix->mat[ it ] + it2 );
	}

	fclose( hfile );
}

// helper function to destroy resources and prevent memory leaks
void destroy_mat( matrix_t *matrix )
{
	for( int it = 0; it < matrix->size; ++it )
		free( matrix->mat[ it ] );

	free( matrix->mat );
	matrix->mat = NULL;
}

// helper function to display contents of matrix
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

// entry point function
int main( int argc, char **argv )
{
	int rank;
	MPI_Init( argc, argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );

	if( rank == 0 ) // parent
	{
		matrix_t matrix;
		init_mat( &matrix, "inmatrix12" );
		print_mat( &matrix );
		destroy_mat( &matrix );
	}
	else // child
	{

	}


	MPI_Finalize();

	return 0;
}