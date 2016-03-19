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

	if( rank == 0 ) // parent
	{
		matrix_t *mat_arr = init_mat_arr( "inmatrix12" );
		print_mat( mat_arr + 0 );
		print_mat( mat_arr + 1 );
		destroy_mat_arr( mat_arr );
		n = mat_arr->size;
	}

	MPI_Bcast( &n, 1, MPI_INT, 0, MPI_COMM_WORLD );
	if( n % p )
	{
		if( p == 0 )
			printf( "matrix size not divisible by proc count!\n" );
		MPI_Finalize();
		exit( 1 );
	}

	MPI_Comm grid2D;
	int dimensions[ 2 ] = { n, n };
	int periodic[ 2 ] = { 0, 0 };
	MPI_Cart_create( MPI_COMM_WORLD, 2, dimensions, periodic, 0, &grid2D );

	MPI_Comm_free( &grid2D );

	MPI_Finalize();

	return 0;
}