#include <string.h>
#include <stdio.h>
#include "mbedtls/md.h"

/** 测试向量来自 https://tools.ietf.org/rfc/rfc4231.txt (test case 2)*/

static char *secret = "Jefe";
static char *msg = "what do ya want for nothing?";
static unsigned char rst[] = 
{
    0x5B, 0xDC, 0xC1, 0x46, 0xBF, 0x60, 0x75, 0x4E, 
    0x6A, 0x04, 0x24, 0x26, 0x08, 0x95, 0x75, 0xC7, 
    0x5A, 0x00, 0x3F, 0x08, 0x9D, 0x27, 0x39, 0x83, 
    0x9D, 0xEC, 0x58, 0xB9, 0x64, 0xEC, 0x38, 0x43
};

int main(void)
{
    unsigned char mac[32];
    mbedtls_md_context_t sha_ctx;

    mbedtls_md_init(&sha_ctx);

    int ret = mbedtls_md_setup(&sha_ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1);
    if (ret != 0)
    {
        printf("  ! mbedtls_md_setup() returned -0x%04x\n", -ret);
        goto exit;
    }

    mbedtls_md_hmac_starts(&sha_ctx, secret, strlen(secret));
    mbedtls_md_hmac_update(&sha_ctx, msg, strlen(msg));
    mbedtls_md_hmac_finish(&sha_ctx, mac );

    printf("\nHMAC(%s) = ", msg);
    for (int i = 0; i < sizeof(mac); i++)
    {
        printf("%02X%s", mac[i], i == sizeof(mac) - 1 ? "\n\n" : "");
    }

exit:
    mbedtls_md_free( &sha_ctx );

    return ret;
}