#include <stdio.h>
#include <string.h>
#include "mbedtls/bignum.h"

int main()
{
	int ret;
    mbedtls_mpi A, X, Y;

    mbedtls_mpi_init( &A ); 
    mbedtls_mpi_init( &X );
    mbedtls_mpi_init( &Y ); 

    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &X, 693 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &Y, 609 ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_gcd( &A, &X, &Y ) );

    printf("gcd(693, 609) = %ld\n", *(A.p));

	if( mbedtls_mpi_cmp_int( &A, 21 ) != 0 )
	{
	    printf( "failed\n");
		goto cleanup;
	}
	else
	{
		printf( "passed\n" );
	}


cleanup:
	mbedtls_mpi_free( &A );
	mbedtls_mpi_free( &X );
    mbedtls_mpi_free( &Y ); 

    return 0;
}