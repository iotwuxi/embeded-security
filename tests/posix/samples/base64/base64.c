#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "mbedtls/base64.h"

/** 测试样本来自 https://en.wikipedia.org/wiki/Base64#Examples */

static char *msg = 
"Man is distinguished, not only by his reason, but by this singular passion from \
other animals, which is a lust of the mind, that by a perseverance of delight \
in the continued and indefatigable generation of knowledge, exceeds the short \
vehemence of any carnal pleasure.";

static char *base64 = 
"TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz\
IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg\
dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu\
dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo\
ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";

int main( void )
{
    size_t len;
    char rst[512];

    printf("\nBase64 encode msg[%ld]: \n%s\n\n", strlen(msg), msg);

    if( mbedtls_base64_encode(rst, sizeof(rst), &len, msg, strlen(msg)) != 0 )
    {
        printf("base64 encode failed\n");
        return -1;
    }
    printf("Base64 encode result[%ld]: \n%s\n\n", len, rst);

    if( mbedtls_base64_decode(rst, sizeof(rst), &len, rst, len) != 0 )
    {
        printf("base64 decode failed\n");
        return -1;
    }

    rst[len] = 0;
    printf("Base64 decode result[%ld]: \n%s\n\n", len, rst);

    return 0;
}