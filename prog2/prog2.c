#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "matrix.h"


// entry point function
int main( int argc, char **argv )
{
	int rank;
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );

	if( rank == 0 ) // parent
	{
		matrix_t *mat_arr = init_mat_arr( "inmatrix12" );
		print_mat( mat_arr + 0 );
		print_mat( mat_arr + 1 );
		destroy_mat_arr( mat_arr );
	}
	else // child
	{

	}


	MPI_Finalize();

	return 0;
}