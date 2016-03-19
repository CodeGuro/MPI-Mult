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
	matrix_t *mat_arr;

	// read the matrix
	if( rank == 0 )
	{
		mat_arr = init_mat_arr( "inmatrix12" );
		n = mat_arr->size;
		print_mat( mat_arr + 0 );
		print_mat( mat_arr + 1 );
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



		destroy_mat_arr( mat_arr );
	}
	else
	{

	}

	MPI_Finalize();

	return 0;
}