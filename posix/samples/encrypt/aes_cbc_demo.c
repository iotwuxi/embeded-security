#include <string.h>
#include <stdio.h>
#include "mbedtls/aes.h"

static char plaintext[] = "Single block msg";

static unsigned char key[16] = {
	0x06, 0xa9, 0x21, 0x40, 0x36, 0xb8, 0xa1, 0x5b, 
	0x51, 0x2e, 0x03, 0xd5, 0x34, 0x12, 0x00, 0x06
};

static unsigned char iv[16] = {
	0x3d, 0xaf, 0xba, 0x42, 0x9d, 0x9e, 0xb4, 0x30, 
	0xb4, 0x22, 0xda, 0x80, 0x2c, 0x9f, 0xac, 0x41
};

static unsigned char ciphertext[16] = {
	0xe3, 0x53, 0x77, 0x9c, 0x10, 0x79, 0xae, 0xb8, 
	0x27, 0x08, 0x94, 0x2d, 0xbe, 0x77, 0x18, 0x1a
};

void mbedtls_aes_cbc_enc_test(void)
{
    int i = 0;
	unsigned char _iv[16];
	unsigned char _key[16];
    unsigned char buff[16];
	mbedtls_aes_context ctx;	
	
	memcpy(_iv, iv, 16);
	memcpy(_key, key, 16);
	memset(buff, 0, 16);

    printf("plaintext : ");
	for(i = 0; i < 16; i++)
    {
        printf("%02x ", plaintext[i]);
    }
    printf("\n");
	
    mbedtls_aes_setkey_enc( &ctx, _key, 128 );
    mbedtls_aes_crypt_cbc( &ctx, MBEDTLS_AES_ENCRYPT, 16, _iv, plaintext, buff );

    printf("ciphertext: ");
    for(i = 0; i < 16; i++)
    {
        printf("%02x ", buff[i]);
    }
    printf("\n");	
}

void mbedtls_aes_cbc_dec_test(void)
{
    int i = 0;
	unsigned char _iv[16];
	unsigned char _key[16];
    unsigned char buff[16];
	mbedtls_aes_context ctx;	
	
	memcpy(_iv, iv, 16);
	memcpy(_key, key, 16);
	memset(buff, 0, 16);

    printf("ciphertext: ");
	for(i = 0; i < 16; i++)
    {
        printf("%02x ", ciphertext[i]);
    }
    printf("\n");
	
    mbedtls_aes_setkey_dec( &ctx, _key, 128 );
    mbedtls_aes_crypt_cbc( &ctx, MBEDTLS_AES_DECRYPT, 16, _iv, ciphertext, buff );

    printf("plaintext : ");
    for(i = 0; i < 16; i++)
    {
        printf("%02x ", buff[i]);
    }
    printf("\n");
}

int main(void)
{
	mbedtls_aes_cbc_enc_test();
	mbedtls_aes_cbc_dec_test();
	
	return 0;
}

