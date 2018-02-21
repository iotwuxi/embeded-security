#include <stdio.h>

#include "mbedtls/md.h"
#include "mbedtls/ssl.h"

#define mbedtls_printf     printf

int main( void )
{
    int index = 1;
    const int *list;

    mbedtls_printf("\nAvailable Ciphersuite:\n");
    list = mbedtls_ssl_list_ciphersuites();
    while (*list) {
        mbedtls_printf("[%02d]%s\n", index++, mbedtls_ssl_get_ciphersuite_name(*list));
        list++; 
    }
    
    return 0;
}