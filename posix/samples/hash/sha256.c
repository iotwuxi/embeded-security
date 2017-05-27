#include <stdio.h>
#include "mbedtls/sha256.h"

int main( void )
{
    int i;
    unsigned char digest[32];
    char str[] = "Hello, world!";

    printf( "\n  SHA256('%s') = ", str );

    mbedtls_sha256( (unsigned char *) str, 13, digest, 0 );

    for( i = 0; i < 32; i++ )
    {
        printf( "%02x", digest[i] );
    }
    printf( "\n\n" );   

    return( 0 );
}

