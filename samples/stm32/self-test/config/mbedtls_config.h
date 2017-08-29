#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H


#define MBEDTLS_SELF_TEST

#if __ICCARM__                                   
#define MBEDTLS_ENTROPY_HARDWARE_ALT            /** 熵源接口替换 */
#define MBEDTLS_NO_PLATFORM_ENTROPY             /** 无平台熵源支持 (linux: /dev/urandom 或 Windows CryptoAPI) */
#endif

#define MBEDTLS_MD_C

#define MBEDTLS_MD2_C
#define MBEDTLS_MD4_C
#define MBEDTLS_MD5_C

#define MBEDTLS_SHA1_C                          /** SHA1 */
#define MBEDTLS_SHA256_C                        /** SHA256 */
#define MBEDTLS_SHA512_C                        /** SHA512 */

#define MBEDTLS_CIPHER_C
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_CIPHER_MODE_CFB
#define MBEDTLS_CIPHER_MODE_CTR

// #define MBEDTLS_DES_C
#define MBEDTLS_AES_C

#define MBEDTLS_BASE64_C

#define MBEDTLS_GCM_C
#define MBEDTLS_CCM_C

// #define MBEDTLS_CMAC_C

#define MBEDTLS_OID_C                           /** X.509 RSA等功能依赖于此定义 */
#define MBEDTLS_ASN1_PARSE_C                    /** X.509 相关功能依赖于此定义 */
#define MBEDTLS_ASN1_WRITE_C                    /** X.509 相关功能依赖于此定义 */
#define MBEDTLS_BIGNUM_C                        /** X.509 ECC RSA等功能依赖于此定义 */

#define MBEDTLS_RSA_C
#define MBEDTLS_PKCS1_V15 
#define MBEDTLS_PKCS1_V21 

#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_HMAC_DRBG_C

#define MBEDTLS_PLATFORM_C
#define MBEDTLS_PLATFORM_MEMORY
#define MBEDTLS_MEMORY_BUFFER_ALLOC_C

#include "mbedtls/check_config.h"

#endif /* MBEDTLS_CONFIG_H */