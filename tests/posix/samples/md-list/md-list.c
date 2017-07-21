#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "mbedtls/md.h"
#include "mbedtls/cipher.h"

int main( void )
{
    const int *list;
    const mbedtls_md_info_t *md_info;    
    const mbedtls_cipher_info_t *cipher_info;

    printf( "Available ciphers:\n" );
    list = mbedtls_cipher_list();
    while( *list )
    {
        cipher_info = mbedtls_cipher_info_from_type( *list );
        printf( "  %s\n", cipher_info->name );
        list++;
    }

    printf( "\nAvailable message digests:\n" );
    list = mbedtls_md_list();
    while( *list )
    {
        md_info = mbedtls_md_info_from_type( *list );
        printf( "  %s\n", mbedtls_md_get_name( md_info ) );
        list++;
    }

    return 0;
}