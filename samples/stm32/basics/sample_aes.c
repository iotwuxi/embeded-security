
#include "basics.h"

static char *msg= "12345678901234567890";

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

static void print_hexdump(unsigned char *output, unsigned int olen)
{
    if (olen == 0) return;
    
    for (int i = 0; i < olen; i++) {
        mbedtls_printf("%02X ", *output++);
    }
    mbedtls_printf("\n");
}

int sample_aes(void)
{
    int ret = 1;
    const mbedtls_cipher_info_t *cipher_info;
    mbedtls_cipher_context_t cipher_ctx;

    mbedtls_printf("\n==========Cipher AES Sample=========\n");
    
    size_t ilen, olen;
    size_t msg_size, offset;
    unsigned char buffer[128];
    unsigned char output[128];

    const int *list;
    mbedtls_printf( "Available ciphers:\n" );
    list = mbedtls_cipher_list();
    while (*list ) {
        cipher_info = mbedtls_cipher_info_from_type((mbedtls_cipher_type_t)*list);
        mbedtls_printf("  %s\n", cipher_info->name);
        list++;
    }

    mbedtls_cipher_init(&cipher_ctx);
    cipher_info = mbedtls_cipher_info_from_string("AES-128-CBC");

    if ((ret = mbedtls_cipher_setup(&cipher_ctx, cipher_info)) != 0) {
        mbedtls_printf("mbedtls_cipher_setup failed\n" );
        goto exit;
    }

    if (mbedtls_cipher_setkey(&cipher_ctx, key, cipher_info->key_bitlen,
                    MBEDTLS_ENCRYPT) != 0 ) {
        mbedtls_printf("mbedtls_cipher_setkey() returned error\n");
        goto exit;
    }

    if (mbedtls_cipher_set_iv(&cipher_ctx, iv, 16) != 0 ) {
        mbedtls_printf("mbedtls_cipher_set_iv() returned error\n" );
        goto exit;
    }

    if (mbedtls_cipher_reset( &cipher_ctx ) != 0) {
        mbedtls_printf("mbedtls_cipher_reset() returned error\n" );
        goto exit;
    }


    msg_size = strlen(msg);
    mbedtls_printf("Message Size: %d\n", msg_size);

    for (offset = 0; offset < msg_size; 
        offset += mbedtls_cipher_get_block_size( &cipher_ctx)) {
        ilen = ((unsigned int) msg_size - offset > mbedtls_cipher_get_block_size( &cipher_ctx)) ?
            mbedtls_cipher_get_block_size( &cipher_ctx ) : (unsigned int) (msg_size - offset);

        memcpy(buffer, msg + offset, ilen);

        if (mbedtls_cipher_update(&cipher_ctx, buffer, ilen, output, &olen) != 0) {
            mbedtls_printf("mbedtls_cipher_update() returned error\n");
            goto exit;
        }

        // 打印
        print_hexdump(output, olen);
    }

    if (mbedtls_cipher_finish( &cipher_ctx, output, &olen ) != 0) {
        mbedtls_printf( "mbedtls_cipher_finish() returned error\n" );
        goto exit;
    }

    print_hexdump(output, olen);

exit:
    mbedtls_cipher_free( &cipher_ctx );
    
    return ret;
}
