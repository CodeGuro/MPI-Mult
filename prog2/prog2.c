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
	matrix_t *m1, *m2, *m3;  // used by root only
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &p );

	p = 4; // debug only

	// read the matrix
	if( rank == 0 )
	{
		FILE *hfile = fopen( "inmatrix12", "r" );
		fscanf( hfile, "%i", &n );
		m1 = alloc_mat( n, hfile );
		m2 = alloc_mat( n, hfile );
		fclose( hfile );

		m3 = alloc_mat( n, NULL );
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

	const int sub_size = n / p; // size of the submatrices
	const int steps = n / sub_size; // number of submatrices to evaluate per process

	// allocate the sub-matrices
	matrix_t *sub1 = alloc_mat( sub_size, NULL );
	matrix_t *sub2 = alloc_mat( sub_size, NULL );
	matrix_t *sub3 = alloc_mat( sub_size, NULL );

	for( int i = 0; i < steps; ++i )
	{
		for( int j = 0; j < steps; ++j )
		{
			zero_mat( sub3 );
			float *mat_row_m1;
			float *mat_col_m2;
			float *adding_mat_set;
			if( rank == 0 )
			{
				// copy the sub-matrices in rows/columns to distribute to each process
				mat_row_m1 = malloc( sub_size*sub_size*steps * sizeof( float ) );
				mat_col_m2 = malloc( sub_size*sub_size*steps * sizeof( float ) );
				adding_mat_set = malloc( sub_size*sub_size*steps * sizeof( float ) );
				for( int k = 0; k < steps; ++k )
				{
					get_sub_mat( sub1, m1, i, k );
					copy_buff_mat( mat_row_m1, sub1, k );
					get_sub_mat( sub2, m2, k, j );
					copy_buff_mat( mat_col_m2, sub2, k );
				}
			}

			// scatter the submatrices
			MPI_Scatter( mat_row_m1, sub_size*sub_size*steps, MPI_FLOAT, sub1, sub_size*sub_size, MPI_FLOAT, 0, MPI_COMM_WORLD );
			MPI_Scatter( mat_col_m2, sub_size*sub_size*steps, MPI_FLOAT, sub2, sub_size*sub_size, MPI_FLOAT, 0, MPI_COMM_WORLD );

			// matrix multiplication is done locally
			multiply_mat( sub3, sub1, sub2 );

			// gather the submatrices into the root process
			MPI_Gather( sub3, sub_size*sub_size, MPI_FLOAT, adding_mat_set, sub_size*sub_size*steps, MPI_FLOAT, 0, MPI_COMM_WORLD );

			if( rank == 0 )
			{
				free( adding_mat_set );
				free( mat_col_m2 );
				free( mat_row_m1 );
			}
		}
	}

	print_mat( m3 );

	// free the resources
	destroy_mat( sub3 );
	destroy_mat( sub2 );
	destroy_mat( sub1 );

	if( rank == 0 )
	{
		destroy_mat( m3 );
		destroy_mat( m2 );
		destroy_mat( m1 );
	}

	MPI_Finalize();

	return 0;
}