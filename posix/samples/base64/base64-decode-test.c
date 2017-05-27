#include <stdio.h>
#define mbedtls_printf     printf

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "mbedtls/base64.h"

static const unsigned char base64_test_enc[] =
    "JEhuVodiWr2/F9mixBcaAZTtjx4Rs9cJDLbpEG8i7hPK"
    "swcFdsn6MWwINP+Nwmw4AEPpVJevUEvRQbqVMVoLlw==";

int main( void )
{
    unsigned char buffer[128];
    size_t len;

    if (mbedtls_base64_decode(buffer, sizeof(buffer), &len, base64_test_enc, sizeof(base64_test_enc)-1) != 0 )
    {
        mbedtls_printf("base64 failed\n");
        return 1;
    }

    mbedtls_printf("base64 decode length: %d\n", len);
    mbedtls_printf("base64 decode result:\n");
    for (int i = 0; i < len; i++)
    {
        mbedtls_printf("%02X ", buffer[i]);
        if ((i+1) % 8 == 0)  mbedtls_printf("\n");
    }
    mbedtls_printf("\n");

    return 0;
}