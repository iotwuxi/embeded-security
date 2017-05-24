#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf     printf
#endif

#include "mbedtls/error.h"
#include "mbedtls/pk.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/rsa.h"
#include "mbedtls/error.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DFL_EC_CURVE            mbedtls_ecp_curve_list()->grp_id
#define DFL_FILENAME            "ecc.key"

#if !defined(MBEDTLS_PK_WRITE_C) || !defined(MBEDTLS_PEM_WRITE_C) || \
    !defined(MBEDTLS_FS_IO) || !defined(MBEDTLS_ENTROPY_C) || \
    !defined(MBEDTLS_CTR_DRBG_C) || !defined(MBEDTLS_ECP_C)
int main( void )
{
    mbedtls_printf( "MBEDTLS_PK_WRITE_C and/or MBEDTLS_FS_IO and/or "
            "MBEDTLS_ENTROPY_C and/or MBEDTLS_CTR_DRBG_C and/or "
            "MBEDTLS_PEM_WRITE_C and/or MBEDTLS_ECP_C"
            "not defined.\n" );
    return( 0 );
}
#else

int main( void )
{
    int ret = 0;
    FILE *fkey = NULL;
    mbedtls_ecp_keypair ecp;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const char *pers = "ecc_genkey";

#if defined(MBEDTLS_ECP_C)
    const mbedtls_ecp_curve_info *curve_info;
        mbedtls_printf( " available ec_curve values:\n" );
        curve_info = mbedtls_ecp_curve_list();
        mbedtls_printf( "    %s (default)\n", curve_info->name );
        while( ( ++curve_info )->name != NULL )
            mbedtls_printf( "    %s\n", curve_info->name );
#endif

    /*
     * Set to sane values
     */
    mbedtls_ecp_keypair_init( &ecp );
    mbedtls_ctr_drbg_init( &ctr_drbg );

    mbedtls_printf( "\n  . Seeding the random number generator..." );
    fflush( stdout );

    mbedtls_entropy_init( &entropy );
    if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                               (const unsigned char *) pers,
                               strlen( pers ) ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret );
        goto exit;
    }

    /*
     * 1.1. Generate the key
     */
    mbedtls_printf( "\n  . Generating the private key ..." );
    fflush( stdout );

    ret = mbedtls_ecp_gen_key( DFL_EC_CURVE, &ecp, mbedtls_ctr_drbg_random, &ctr_drbg );
    if( ret != 0 )
    {
        mbedtls_printf( " failed\n  !  mbedtls_ecp_gen_key returned -0x%04x", -ret );
        goto exit;
    }

    /*
     * 1.2 Print the key
     */
    mbedtls_printf( " ok\n  . Key information:\n" );

    mbedtls_printf( "curve: %s\n",
            mbedtls_ecp_curve_info_from_grp_id( ecp.grp.id )->name );
    mbedtls_mpi_write_file( "XQ = ", &ecp.Q.X, 16, NULL );
    mbedtls_mpi_write_file( "YQ = ", &ecp.Q.Y, 16, NULL );
    mbedtls_mpi_write_file( "D = ", &ecp.d, 16, NULL );

    /*
     * 1.2 Write to file
     */
    mbedtls_printf( "  . Exporting the ecc key in %s...",  DFL_FILENAME);
    fflush( stdout );

    if( ( fkey = fopen( DFL_FILENAME, "wb+" ) ) == NULL )
    {
        mbedtls_printf( " failed\n  ! could not open %s for writing\n", DFL_FILENAME );
        ret = 1;
        goto exit;
    }
    if( ( ret = mbedtls_mpi_write_file( "XQ = ", &ecp.Q.X, 16, fkey ) ) != 0 ||
        ( ret = mbedtls_mpi_write_file( "YQ = ", &ecp.Q.Y, 16, fkey ) ) != 0 ||
        ( ret = mbedtls_mpi_write_file( "D = ", &ecp.d, 16, fkey ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_mpi_write_file returned %d\n\n", ret );
        goto exit;
    }

    mbedtls_printf( " ok\n\n" );

exit:

    mbedtls_ecp_keypair_free( &ecp );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );

    return( ret );
}
#endif /* MBEDTLS_PK_WRITE_C && MBEDTLS_PEM_WRITE_C && MBEDTLS_FS_IO &&
        * MBEDTLS_ENTROPY_C && MBEDTLS_CTR_DRBG_C */

