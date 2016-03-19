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

	n = p = 3; // debug only

	// read the matrix
	if( rank == 0 )
	{
		FILE *hfile = fopen( "inmatrix12", "r" );
		fscanf( hfile, "%i", &n );
		m1 = alloc_mat( n, hfile );
		m2 = alloc_mat( n, hfile );
		m3 = alloc_mat( n, hfile );
		fclose( hfile );
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

	int steps = n / p; // number of submatrices to evaluate per process
	int sub_size = n / steps; // size of the submatrices

	// allocate the sub-matrices
	matrix_t *sub1 = alloc_mat( sub_size, NULL );
	matrix_t *sub2 = alloc_mat( sub_size, NULL );
	matrix_t *sub3 = alloc_mat( sub_size, NULL );

	for( int i = 0; i < steps; ++i )
	{
		for( int j = 0; j < steps; ++j )
		{
			zero_mat( sub3 );
			for( int k = 0; k < steps; ++k )
			{
				// non-parallel part
				get_sub_mat( sub1, m1, i, k );
				get_sub_mat( sub2, m2, k, j );
				matrix_t *tmp = alloc_mat( sub_size, NULL );
				multiply_mat( tmp, sub1, sub2 );
				add_mat( sub3, sub3, tmp );
				destroy_mat( tmp );
			}
			set_sub_mat( m3, sub3, i, j );
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