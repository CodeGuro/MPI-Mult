#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "matrix.h"


// entry point function
int main( int argc, char **argv )
{
	int rank;
	int size;
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );

	if( rank == 0 ) // parent
	{
		matrix_t *mat_arr = init_mat_arr( "inmatrix12" );
		print_mat( mat_arr + 0 );
		print_mat( mat_arr + 1 );
		destroy_mat_arr( mat_arr );

		if( mat_arr[ 0 ].size % size )
		{
			printf( "Matrix size not divisible by process count!\nexiting\n" );
			MPI_Finalize();
			return 0;
		}

		int num = mat_arr[ 0 ].size / size; 
	}
	else // child
	{
	}

/*	int buff[] = { 1, 2, 3, 4, 5, 6 };
	int recbuff[ 3 ];
	MPI_Scatter( buff, 3, MPI_INT, recbuff, 3, MPI_INT, 0, MPI_COMM_WORLD );

	printf( "proc %i recieved %i %i %i\n", rank, recbuff[ 0 ], recbuff[ 1 ], recbuff[ 2 ] );
*/


	MPI_Finalize();

	return 0;
}