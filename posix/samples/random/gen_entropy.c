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

#if defined(MBEDTLS_ENTROPY_C) && defined(MBEDTLS_FS_IO)
#include "mbedtls/entropy.h"

#include <stdio.h>
#endif

#if !defined(MBEDTLS_ENTROPY_C) || !defined(MBEDTLS_FS_IO)
int main( void )
{
    mbedtls_printf("MBEDTLS_ENTROPY_C and/or MBEDTLS_FS_IO not defined.\n");
    return( 0 );
}
#else
#define ENTROPY_FILE "entropy"
int main( void )
{
    FILE *f;
    int ret;
    mbedtls_entropy_context entropy;
    unsigned char buf[MBEDTLS_ENTROPY_BLOCK_SIZE];

    if( ( f = fopen( ENTROPY_FILE, "wb+" ) ) == NULL )
    {
        mbedtls_printf( "failed to open '%s' for writing.\n", ENTROPY_FILE );
        return( 1 );
    }

    mbedtls_entropy_init( &entropy );

    ret = mbedtls_entropy_func( &entropy, buf, sizeof( buf ) );
    if( ret != 0 )
    {
        mbedtls_printf("failed!\n");
        goto cleanup;
    }

    fwrite( buf, 1, sizeof( buf ), f );

    mbedtls_printf( "Generating %ldkb of data in file '%s' done.\r", sizeof(buf), ENTROPY_FILE );
    fflush( stdout );

    ret = 0;

cleanup:
    mbedtls_printf( "\n" );

    fclose( f );
    mbedtls_entropy_free( &entropy );

    return( ret );
}
#endif /* MBEDTLS_ENTROPY_C */
