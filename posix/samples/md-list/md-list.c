
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


#include "mbedtls/md.h"
#include "mbedtls/cipher.h"

int main( void )
{
    int ret = 0;
    const mbedtls_cipher_info_t *cipher_info;
    const mbedtls_md_info_t *md_info;

    const int *list;
    mbedtls_printf( "Available ciphers:\n" );
    list = mbedtls_cipher_list();
    while( *list )
    {
        cipher_info = mbedtls_cipher_info_from_type( *list );
        mbedtls_printf( "  %s\n", cipher_info->name );
        list++;
    }

    mbedtls_printf( "\nAvailable message digests:\n" );
    list = mbedtls_md_list();
    while( *list )
    {
        md_info = mbedtls_md_info_from_type( *list );
        mbedtls_printf( "  %s\n", mbedtls_md_get_name( md_info ) );
        list++;
    }

    return ret;
}