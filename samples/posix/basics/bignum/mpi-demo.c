/*
输出结果
  Public key:

N = 884F2F
E = 0101

  Private key:

P = 0AE5
Q = 0C83
D = E^-1 mod (P-1)*(Q-1) = 607731

  RSA operation:

X (plaintext)  = D903
Y (ciphertext) = X^E mod N = 56629B
Z (decrypted)  = Y^D mod N = D903

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mbedtls/bignum.h"

#define BUF_LEN 512

int main( void )
{
    int ret = -1;
    mbedtls_mpi E, P, Q, N, H, D, X, Y, Z;
    char buffer[BUF_LEN];
    size_t olen;

    mbedtls_mpi_init( &E );
    mbedtls_mpi_init( &P );
    mbedtls_mpi_init( &Q );
    mbedtls_mpi_init( &N );
    mbedtls_mpi_init( &H );
    mbedtls_mpi_init( &D );
    mbedtls_mpi_init( &X );
    mbedtls_mpi_init( &Y );
    mbedtls_mpi_init( &Z );

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &P, 10, "2789" ) );   // P Q 很大的质数
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &Q, 10, "3203" ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &E, 16,  "101" ) );   // E 满足 gcd(E, L)  
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &N, &P, &Q ) );           // N = P X Q

    printf("\n  Public key:\n\n" );
    MBEDTLS_MPI_CHK( mbedtls_mpi_write_string( &N, 16, buffer, BUF_LEN, &olen ) );
    printf("N = %s\n", buffer);
    MBEDTLS_MPI_CHK( mbedtls_mpi_write_string( &E, 16, buffer, BUF_LEN, &olen ) );
    printf("E = %s\n", buffer);

    printf("\n  Private key:\n\n" );
    MBEDTLS_MPI_CHK( mbedtls_mpi_write_string( &P, 16, buffer, BUF_LEN, &olen ) );
    printf("P = %s\n", buffer);

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_string( &Q, 16, buffer, BUF_LEN, &olen ) );
    printf("Q = %s\n", buffer);

#if defined(MBEDTLS_GENPRIME)
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_int( &P, &P, 1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_int( &Q, &Q, 1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &H, &P, &Q ) );   // H = (P-1) X (Q - 1)
    MBEDTLS_MPI_CHK( mbedtls_mpi_inv_mod( &D, &E, &H ) );   // D =  E ^ -1 mode H => D X E mod H =1

    mbedtls_mpi_write_string( &D, 16, buffer, BUF_LEN, &olen );
    printf("D = E^-1 mod (P-1)*(Q-1) = %s\n", buffer);
#else
    printf("\nTest skipped (MBEDTLS_GENPRIME not defined).\n\n");
#endif
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &X, 10, "123" ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_exp_mod( &Y, &X, &E, &N, NULL ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_exp_mod( &Z, &Y, &D, &N, NULL ) );

    printf("\n  RSA operation:\n\n" );
    MBEDTLS_MPI_CHK( mbedtls_mpi_write_string( &X, 16, buffer, BUF_LEN, &olen ) );
    printf("X (plaintext)  = %s\n", buffer);
    MBEDTLS_MPI_CHK( mbedtls_mpi_write_string( &Y, 16, buffer, BUF_LEN, &olen ) );
    printf("Y (ciphertext) = X^E mod N = %s\n", buffer);
    MBEDTLS_MPI_CHK( mbedtls_mpi_write_string( &Z, 16, buffer, BUF_LEN, &olen ) );
    printf("Z (decrypted)  = Y^D mod N = %s\n\n", buffer);

    ret = 0;

cleanup:
    mbedtls_mpi_free( &E );
    mbedtls_mpi_free( &P );
    mbedtls_mpi_free( &Q );
    mbedtls_mpi_free( &N );
    mbedtls_mpi_free( &H );
    mbedtls_mpi_free( &D );
    mbedtls_mpi_free( &X );
    mbedtls_mpi_free( &Y );
    mbedtls_mpi_free( &Z );

    if ( ret != 0 ) {
        printf("\nAn error occurred.\n" );
        ret = -1;
    }

    return ( ret );
}

