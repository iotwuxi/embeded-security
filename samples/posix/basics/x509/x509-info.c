
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
#define mbedtls_fprintf    fprintf
#define mbedtls_time_t     time_t
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"

int main( void )
{
    int ret;
    char buf[1024];
    mbedtls_x509_crt srvcert;

    mbedtls_x509_crt_init( &srvcert );
    ret = mbedtls_x509_crt_parse( &srvcert, (const unsigned char *) mbedtls_test_srv_crt,
                          mbedtls_test_srv_crt_len );

    if( ret != 0 )
    {
        mbedtls_printf( " failed\n  !  mbedtls_x509_crt_parse returned %d\n\n", ret );
        goto exit;
    }

    mbedtls_x509_crt_info( buf, sizeof(buf) - 1, "", &srvcert);
    mbedtls_printf( "%s", buf );

exit:
    mbedtls_x509_crt_free( &srvcert );

    return 0;
}

