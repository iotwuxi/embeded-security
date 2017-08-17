
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "otp.h"
#include "mbedtls/md.h"
     
// #define DEBUG   1

static int DIGITS_POWER[] = 
        {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

// 默认单向散列算法
mbedtls_md_type_t m_md_type = MBEDTLS_MD_SHA1;
// 生成口令长度
uint8_t m_num = 6;

static int32_t truncate(uint8_t *hmac, int8_t num, mbedtls_md_type_t md_type)
{
    // 最后一个字节的低4位
    int offset = 0;
    int size = 0;
    size = mbedtls_md_get_size(mbedtls_md_info_from_type(md_type));

    offset = hmac[size - 1] & 0x0f;

    int32_t bin_code = ((hmac[offset] & 0x7f) << 24) |
                    ((hmac[offset + 1] & 0xff) << 16) | 
                    ((hmac[offset + 2] & 0xff) << 8) | 
                    ((hmac[offset + 3] & 0xff));

    int32_t token = bin_code % DIGITS_POWER[num];

    return token;
}

void print_hmac(uint8_t *digest)
{
    int size = mbedtls_md_get_size(mbedtls_md_info_from_type(m_md_type));
    printf("HMAC: ");
    for (int i = 0; i < size; i++) {
        printf("%02X", *digest++);
    }
    printf("\n");
}

// 需要检查
static uint8_t compute_hmac(
    uint8_t *digest,
    uint8_t *key, uint8_t key_len,
    uint8_t *data, uint8_t data_len)
{
    uint8_t ret = 0;
    mbedtls_md_context_t sha_ctx;
    mbedtls_md_init(&sha_ctx);

    ret = mbedtls_md_setup(&sha_ctx, mbedtls_md_info_from_type(m_md_type), 1);
    if (ret != 0)
    {
        goto exit;
    }

    mbedtls_md_hmac_starts(&sha_ctx, key, key_len);
    mbedtls_md_hmac_update(&sha_ctx, data, data_len);
    mbedtls_md_hmac_finish(&sha_ctx, digest);
    
#if DEBUG
    print_hmac(digest);
#endif

exit:
    mbedtls_md_free(&sha_ctx);
    return ret;
}

void hotp_set_num(uint8_t num)
{
    m_num = num;
}

void hotp_md_set(mbedtls_md_type_t type)
{
    m_md_type = type;
}

mbedtls_md_type_t hotp_md_get(void)
{
    return m_md_type;
}

/*
 * @brief: 生成HOTP
 */
int32_t hotp_gen(uint8_t *key, uint8_t key_len, 
                 int64_t counter)
{
    uint8_t size = mbedtls_md_get_size(mbedtls_md_info_from_type(m_md_type));
    uint8_t *hmac = (uint8_t*)malloc(size);
    uint8_t *data = (uint8_t*)malloc(sizeof(uint64_t));
    uint8_t data_len = sizeof(uint64_t);
    
    for(int i = 7; i >= 0; i--)
    {
        data[i] = (uint8_t)(counter & 0xff);
        counter >>= 8;
    }

    compute_hmac(hmac, key, key_len, data, data_len);

    uint32_t token = truncate(hmac, m_num, m_md_type);
    
    free(hmac);
    free(data);
    return token;
}