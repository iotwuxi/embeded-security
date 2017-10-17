#include <stdio.h>
#include <string.h>
#include "mbedtls/bignum.h"

#include "basics.h"

int gcd(void)
{
	int ret;
    mbedtls_mpi A, X, Y;

    mbedtls_mpi_init( &A ); 
    mbedtls_mpi_init( &X );
    mbedtls_mpi_init( &Y ); 

    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &X, 693 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &Y, 609 ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_gcd( &A, &X, &Y ) );

    // 正确答案为 21
    mbedtls_printf("gcd(693, 609) = %ld\n", *(A.p));

cleanup:
	mbedtls_mpi_free( &A );
	mbedtls_mpi_free( &X );
    mbedtls_mpi_free( &Y ); 
    
    return ret;
}

int inv(void)
{
    // 示例来自于 《深入浅出现代密码学》 P154 示例6.6 扩展欧几里得算法
    // 28 * 12 = 336 % 67 = 1
    int ret;
    mbedtls_mpi X, A, N;

    mbedtls_mpi_init( &X ); 
    mbedtls_mpi_init( &A );
    mbedtls_mpi_init( &N ); 

    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &A, 12 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &N, 67 ) );

    // X = A^-1 mod N
    MBEDTLS_MPI_CHK( mbedtls_mpi_inv_mod( &X, &A, &N ));
    
    // 正确答案为28
    mbedtls_printf("12^-1mod67 = %ld\n", *(X.p));
    
cleanup:
	mbedtls_mpi_free( &X );
	mbedtls_mpi_free( &A );
    mbedtls_mpi_free( &N ); 
    return ret;
}

int sample_smallint()
{
    // 计算最大公约数
    gcd();
    
    // 计算模你愿
    inv();
    
    return 0;
}