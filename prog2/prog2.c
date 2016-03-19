#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "matrix.h"


// entry point function
int main( int argc, char **argv )
{
	int rank;
	int p;
	int n; // matrix size
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &p );

	// read the matrix
	if( rank == 0 )
	{
		FILE *hfile = fopen( "inmatrix12", "r" );
		fscanf( hfile, "%i", &n );
		matrix_t *m1 = alloc_mat( n, hfile );
		matrix_t *m2 = alloc_mat( n, hfile );
		matrix_t *m3 = alloc_mat( n, NULL );
		fclose( hfile );
		//print_mat( m1 );
		//print_mat( m2 );
		multiply_mat( m3, m1, m2 );
		print_mat( m3 );
		destroy_mat( m3 );
		destroy_mat( m2 );
		destroy_mat( m1 );
	}

	// broadcast the size of the matrix
	MPI_Bcast( &n, 1, MPI_INT, 0, MPI_COMM_WORLD );

	// exit if p doesn't divide n
	if( n % p )
	{
		if( p == 0 )
			printf( "matrix size not divisible by proc count!\n" );
		MPI_Finalize();
		exit( 1 );
	}

	int steps = p / n; // number of submatrices to evaluate per process

	if( rank == 0 )
	{
		// partition the matrix and broadcast it to the processes
	}
	else
	{

	}

	MPI_Finalize();

	return 0;
}