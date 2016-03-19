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
	FILE *hfout = NULL;
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &p );

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
		if( rank == 0 )
			printf( "matrix size not divisible by proc count!\n" );
		MPI_Finalize();
		exit( 1 );
	}
	else if( rank == 0 )
	{
		char filename_buff[ 512 ] = { 0 };
		sprintf( filename_buff, "outfile%i", n );
		hfout = fopen( filename_buff, "w" );
		print_mat( m1, "Matrix A", hfout );
		print_mat( m2, "Matrix B", hfout );
	}

	const int sub_size = n / p; // size of the submatrices
	const int steps = n / sub_size; // number of submatrices to evaluate per process

	// allocate the sub-matrices
	matrix_t *sub1 = alloc_mat( sub_size, NULL );
	matrix_t *sub2 = alloc_mat( sub_size, NULL );
	matrix_t *sub3 = alloc_mat( sub_size, NULL );

	// root-specific vars
	float *mat_set_rows;
	float *mat_set_cols;
	float *mat_set_Cvec;

	if( rank == 0 )
	{
		mat_set_rows = malloc( sub_size*sub_size * steps * sizeof( float ) );
		mat_set_cols = malloc( sub_size*sub_size * steps * sizeof( float ) );
		mat_set_Cvec = malloc( sub_size*sub_size * steps * sizeof( float ) );
	}

	for( int i = 0; i < steps; ++i )
	{
		// initialize the matrix rows so process k gets submatrix A(i,k)
		if( rank == 0 )
		{
			matrix_t *tmp = alloc_mat( sub_size, NULL );
			for( int k = 0; k < steps; ++k )
			{
				get_sub_mat( tmp, m1, i, k );
				copy_buff_mat( mat_set_rows, tmp, k );
			}
			destroy_mat( tmp );
		}

		// scatter the matrix rows
		MPI_Scatter( rank == 0 ? mat_set_rows : NULL, sub_size*sub_size, MPI_FLOAT, sub1->mat, sub_size*sub_size, MPI_FLOAT, 0, MPI_COMM_WORLD );

		// initialize the matrix rows so process k gets submatrix B(k,j)
		for( int j = 0; j < steps; ++j )
		{
			if( rank == 0 )
			{
				matrix_t *tmp = alloc_mat( sub_size, NULL );
				for( int k = 0; k < steps; ++k )
				{
					get_sub_mat( tmp, m2, k, j );
					copy_buff_mat( mat_set_cols, tmp, k );
				}
				destroy_mat( tmp );
			}

			// scatter the matrix columns
			MPI_Scatter( rank == 0 ? mat_set_cols : NULL, sub_size*sub_size, MPI_FLOAT, sub2->mat, sub_size*sub_size, MPI_FLOAT, 0, MPI_COMM_WORLD );

			// perform matrix multiplication locally
			multiply_mat( sub3, sub1, sub2 );

			// gather c-set at root
			MPI_Gather( sub3->mat, sub_size*sub_size, MPI_FLOAT, rank == 0 ? mat_set_Cvec : NULL, sub_size*sub_size, MPI_FLOAT, 0, MPI_COMM_WORLD );

			// add the c-set as a vector of matrices, store the result in submatrix C(i,j)
			if( rank == 0 )
			{
				zero_mat( sub3 );
				matrix_t *tmp = alloc_mat( sub_size, NULL );
				for( int k = 0; k < steps; ++k )
				{
					copy_mat_buff( mat_set_Cvec, tmp, k );
					add_mat( sub3, tmp, sub3 );
				}
				destroy_mat( tmp );

				set_sub_mat( m3, sub3, i, j ); // C(i,j) = summed_submatrix
			}
		}
	}

	if( rank == 0 )
	{
		free( mat_set_Cvec );
		free( mat_set_cols );
		free( mat_set_rows );
	}

	// free the resources
	destroy_mat( sub3 );
	destroy_mat( sub2 );
	destroy_mat( sub1 );

	if( rank == 0 )
	{
		print_mat( m3, "Product Matrix C", hfout );
		fclose( hfout );
		destroy_mat( m3 );
		destroy_mat( m2 );
		destroy_mat( m1 );
	}

	MPI_Finalize();

	return 0;
}