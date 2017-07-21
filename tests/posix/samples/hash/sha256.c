#include <stdio.h>
#include <string.h>
#include "mbedtls/sha256.h"

static char *msg = "Hello, world!";

int main(void)
{
    unsigned char digest[32];

    mbedtls_sha256(msg, strlen(msg), digest, 0);

    printf("\n  SHA256('%s') = ", msg);
    for(int i = 0; i < 32; i++)
    {
        printf("%02X%s", digest[i], i == sizeof(digest) - 1 ? "\n\n" : "");
    }

    return 0;
}

