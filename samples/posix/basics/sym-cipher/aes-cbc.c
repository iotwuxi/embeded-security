#include <string.h>
#include <stdio.h>
#include "mbedtls/aes.h"

/** 测试向量来自 https://www.rfc-editor.org/rfc/rfc3602.txt (Case #1) */

static unsigned char msg[16] = "Single block msg";

static unsigned char key[16] = 
{
    0x06, 0xa9, 0x21, 0x40, 0x36, 0xb8, 0xa1, 0x5b, 
    0x51, 0x2e, 0x03, 0xd5, 0x34, 0x12, 0x00, 0x06
};

static unsigned char iv[16] = 
{
    0x3d, 0xaf, 0xba, 0x42, 0x9d, 0x9e, 0xb4, 0x30, 
    0xb4, 0x22, 0xda, 0x80, 0x2c, 0x9f, 0xac, 0x41
};

static unsigned char rst[16] = 
{
    0xe3, 0x53, 0x77, 0x9c, 0x10, 0x79, 0xae, 0xb8, 
    0x27, 0x08, 0x94, 0x2d, 0xbe, 0x77, 0x18, 0x1a
};

int main(void)
{
    mbedtls_aes_context ctx; 
    unsigned char temp_iv[16];

    for(int i = 0; i < 16; i++)
    {
        printf("%s%02x %s", i == 0 ? "AES-CBC-128 Msg: " : "", msg[i], i == 15 ? "\n":"");
    }
    
    memcpy(temp_iv, iv, 16);
    mbedtls_aes_setkey_enc( &ctx, key, 128 );
    mbedtls_aes_crypt_cbc( &ctx, MBEDTLS_AES_ENCRYPT, 16, temp_iv, msg, msg );

    for(int i = 0; i < 16; i++)
    {
        printf("%s%02x %s", i == 0 ? "AES-CBC-128 Enc: " : "", msg[i], i == 15 ? "\n":"");
    }

    memcpy(temp_iv, iv, 16);
    mbedtls_aes_setkey_dec( &ctx, key, 128 );
    mbedtls_aes_crypt_cbc( &ctx, MBEDTLS_AES_DECRYPT, 16, iv, msg, msg );

    for(int i = 0; i < 16; i++)
    {
        printf("%s%02x %s", i == 0 ? "AES-CBC-128 Dec: " : "", msg[i], i == 15 ? "\n":"");
    }
    
    return 0;
}

