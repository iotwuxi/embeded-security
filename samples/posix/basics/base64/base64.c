#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "mbedtls/base64.h"

#define mbedtls_printf     printf

// 样本来自于 https://tools.ietf.org/html/rfc3548
// The Base16, Base32, and Base64 Data Encodings
static uint8_t msg[] = {0x14, 0xfb, 0x9c, 0x03, 0xd9, 0x7e};

int main(void)
{
    size_t len;
    uint8_t rst[512];

    len = sizeof(msg)/ sizeof(msg[0]);
    mbedtls_printf("\nBase64 Encode Message[%ld]: ", len);
    for (uint8_t i = 0; i < len; i++) {
        mbedtls_printf("%02x ", msg[i]);
    }

    if (mbedtls_base64_encode(rst, sizeof(rst), &len, msg, len) != 0) {
        mbedtls_printf("\nBase64 Encode Failed\n");
        return -1;
    }
    mbedtls_printf("\nBase64 Encode Result[%ld]: %s", len, rst);

    if (mbedtls_base64_decode(rst, sizeof(rst), &len, rst, len) != 0 ) {
        mbedtls_printf("\nBase64 Decode failed\n");
        return -1;
    }

    mbedtls_printf("\nBase64 Decode Result[%ld]: ", len);
    for (uint8_t i = 0; i < len; i++) {
        mbedtls_printf("%02x ", rst[i]);
    }

    return 0;
}