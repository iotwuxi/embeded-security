#include <string.h>
#include <stdio.h>
#include "mbedtls/aes.h"

/** http://csrc.nist.gov/archive/aes/rijndael/rijndael-vals.zip */
/*
KEYSIZE=128
KEY=000102030405060708090A0B0C0D0E0F

PT=000102030405060708090A0B0C0D0E0F
CT=0A940BB5416EF045F1C39458C653EA5A
*/

static unsigned char msg[16] = 
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

static unsigned char key[16] = 
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};


int main(void)
{
    mbedtls_aes_context ctx; 
    unsigned char cipher[16];
    unsigned char plain[16];

    for(int i = 0; i < 16; i++)
    {
        printf("%s%02x %s", i == 0 ? "AES-ECB-128 Msg: " : "", msg[i], i == 15 ? "\n":"");
    }

    mbedtls_aes_setkey_enc( &ctx, key, 128 );
    mbedtls_aes_crypt_ecb( &ctx, MBEDTLS_AES_ENCRYPT, msg, cipher);

    for(int i = 0; i < 16; i++)
    {
        printf("%s%02x %s", i == 0 ? "AES-ECB-128 Enc: " : "", cipher[i], i == 15 ? "\n":"");
    }

    mbedtls_aes_setkey_dec( &ctx, key, 128 );
    mbedtls_aes_crypt_ecb( &ctx, MBEDTLS_AES_DECRYPT, cipher, plain);

    for(int i = 0; i < 16; i++)
    {
        printf("%s%02x %s", i == 0 ? "AES-ECB-128 Dec: " : "", plain[i], i == 15 ? "\n":"");
    }

    return 0;
}

