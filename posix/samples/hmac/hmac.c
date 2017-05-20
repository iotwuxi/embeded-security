
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// #include "mbedtls/certs.h"
// #include "mbedtls/x509.h"
// #include "mbedtls/ssl.h"

#include "mbedtls/md.h"

int main( void )
{
    int ret;
    // unsigned char digest[32];
    // unsigned char buffer[1024];
    unsigned char secret[] = "a secret";
    unsigned char buffer[] = "some data to hash";
    unsigned char digest[32];
    // mbedtls_aes_context aes_ctx;
    mbedtls_md_context_t sha_ctx;

    mbedtls_md_init(&sha_ctx);
    memset(digest, 0x00, sizeof(digest));

    ret = mbedtls_md_setup( &sha_ctx, mbedtls_md_info_from_type( MBEDTLS_MD_SHA256 ), 1 );
    if( ret != 0 )
    {
        mbedtls_printf( "  ! mbedtls_md_setup() returned -0x%04x\n", -ret );
        goto exit;
    }

    mbedtls_md_hmac_starts( &sha_ctx, secret, sizeof(secret) - 1 );
    mbedtls_md_hmac_update( &sha_ctx, buffer, sizeof(buffer) - 1 );
    mbedtls_md_hmac_finish( &sha_ctx, digest );

    for( int i = 0; i < sizeof(digest); i++ )
        mbedtls_printf( "%02x", digest[i] );

    mbedtls_printf( "\n\n" );

exit:
    // memset( buffer, 0, sizeof( buffer ) );
    // memset( digest, 0, sizeof( digest ) );

    mbedtls_md_free( &sha_ctx );

    return ret;
}