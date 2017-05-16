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

#if defined(MBEDTLS_SHA256_C)
#include "mbedtls/sha256.h"
#endif

#if !defined(MBEDTLS_SHA256_C)
int main( void )
{
    mbedtls_printf("MBEDTLS_SHA256_C not defined.\n");
    return( 0 );
}
#else
int main( void )
{
    int i;
    unsigned char digest[32];
    char str[] = "Hello, world!";

    mbedtls_printf( "\n  SHA256('%s') = ", str );

    mbedtls_sha256( (unsigned char *) str, 13, digest, 0 );

    for( i = 0; i < 32; i++ )
        mbedtls_printf( "%02x", digest[i] );

    mbedtls_printf( "\n\n" );   

    return( 0 );
}
#endif /* MBEDTLS_SHA256_C */

