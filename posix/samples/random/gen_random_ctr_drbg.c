#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_fprintf    fprintf
#define mbedtls_printf     printf
#endif

#if defined(MBEDTLS_CTR_DRBG_C) && defined(MBEDTLS_ENTROPY_C) && \
 defined(MBEDTLS_FS_IO)
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

#include <stdio.h>
#endif

#if !defined(MBEDTLS_CTR_DRBG_C) || !defined(MBEDTLS_ENTROPY_C) || \
 !defined(MBEDTLS_FS_IO)
int main( void )
{
    mbedtls_printf("MBEDTLS_CTR_DRBG_C and/or MBEDTLS_ENTROPY_C and/or MBEDTLS_FS_IO not defined.\n");
    return( 0 );
}
#else
#define RANDOM_FILE "random_ctr"
int main( void )
{
    FILE *f;
    int i, k, ret;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    unsigned char buf[1024];

    mbedtls_ctr_drbg_init( &ctr_drbg );

    if( ( f = fopen( RANDOM_FILE, "wb+" ) ) == NULL )
    {
        mbedtls_printf( "failed to open '%s' for writing.\n", RANDOM_FILE );
        return( 1 );
    }

    mbedtls_entropy_init( &entropy );
    ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *) "RANDOM_GEN", 10 );
    if( ret != 0 )
    {
        mbedtls_printf( "failed in mbedtls_ctr_drbg_seed: %d\n", ret );
        goto cleanup;
    }
    mbedtls_ctr_drbg_set_prediction_resistance( &ctr_drbg, MBEDTLS_CTR_DRBG_PR_OFF );

#if defined(MBEDTLS_FS_IO)
    ret = mbedtls_ctr_drbg_update_seed_file( &ctr_drbg, "seedfile" );

    if( ret == MBEDTLS_ERR_CTR_DRBG_FILE_IO_ERROR )
    {
        mbedtls_printf( "Failed to open seedfile. Generating one.\n" );
        ret = mbedtls_ctr_drbg_write_seed_file( &ctr_drbg, "seedfile" );
        if( ret != 0 )
        {
            mbedtls_printf( "failed in mbedtls_ctr_drbg_write_seed_file: %d\n", ret );
            goto cleanup;
        }
    }
    else if( ret != 0 )
    {
        mbedtls_printf( "failed in mbedtls_ctr_drbg_update_seed_file: %d\n", ret );
        goto cleanup;
    }
#endif

    for( i = 0, k = 768; i < k; i++ )
    {
        ret = mbedtls_ctr_drbg_random( &ctr_drbg, buf, sizeof( buf ) );
        if( ret != 0 )
        {
            mbedtls_printf("failed!\n");
            goto cleanup;
        }

        fwrite( buf, 1, sizeof( buf ), f );

        mbedtls_printf( "Generating %ldkb of data in file '%s'... %04.1f" \
                "%% done\r", (long)(sizeof(buf) * k / 1024), RANDOM_FILE, (100 * (float) (i + 1)) / k );
        fflush( stdout );
    }

    ret = 0;

cleanup:
    mbedtls_printf("\n");

    fclose( f );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );

    return( ret );
}
#endif /* MBEDTLS_CTR_DRBG_C && MBEDTLS_ENTROPY_C */
