#ifndef STM32F7_MBEDTLS_CONFIG_H
#define STM32F7_MBEDTLS_CONFIG_H

/** Debug */
#define MBEDTLS_DEBUG_C							/** 调试(需要修改 DEBUG_LEVEL 值，默认为0) */

/** System support */
#define MBEDTLS_NET_C 							/** 网络接口(net_sockets.c) */
#define MBEDTLS_HAVE_ASM						/** 开启 asm 支持 */
#define MBEDTLS_HAVE_TIME						/** 开启 time() 支持 */
// #define MBEDTLS_TIMING_C						/** 开启 timer 支持，嵌入式平台需要修改 mbedtls_ssl_set_timer_cb 接口 */

/** mbedtls feature support */
#define MBEDTLS_SSL_TLS_C						/** 开启 TLS, 生成 SSL 相关代码(ssl_tls.c ssl_cli.c ssl_srv.c) */
#define MBEDTLS_SSL_CLI_C						/** 开启 SSL Client(ssl_cli.c) */
#define MBEDTLS_SSL_SRV_C						/** 开启 SSL Server(ssl_srv.c) */
#define MBEDTLS_SSL_COOKIE_C					/** Cookie，防DOC攻击(HelloVerifyRequest) */
#define MBEDTLS_SSL_DTLS_HELLO_VERIFY			/** HelloVerify */
#define MBEDTLS_SSL_PROTO_DTLS					/** DTLS 协议 */
#define MBEDTLS_SSL_PROTO_TLS1_2				/** 选择 TLS 协议版本为1.2 */


/** mbedtls bignum modules */
#define MBEDTLS_OID_C  							/** X.509 RSA等功能依赖于此定义 */
#define MBEDTLS_ASN1_PARSE_C					/** X.509 相关功能依赖于此定义 */
#define MBEDTLS_ASN1_WRITE_C					/** X.509 相关功能依赖于此定义 */
#define MBEDTLS_BIGNUM_C						/** X.509 ECC RSA等功能依赖于此定义 */

/** mbedtls cipher modules */
#define MBEDTLS_AES_C 							/** AES 加密 */
#define MBEDTLS_CCM_C                           /** CCM 模式 */
#define MBEDTLS_GCM_C							/** GCM 模式 */
#define MBEDTLS_CIPHER_C						/** 开启 加密模块接口 */
#define MBEDTLS_ENTROPY_C						/** 熵 */
#define MBEDTLS_NO_PLATFORM_ENTROPY				/** 非Linux平台 */
#define MBEDTLS_ENTROPY_HARDWARE_ALT			/** 随机数 */
#define MBEDTLS_TIMING_ALT						/** TIME */
#define MBEDTLS_CTR_DRBG_C 						/** 随机数发生器 */
#define MBEDTLS_SHA1_C 							/** SHA1 */
#define MBEDTLS_SHA256_C 						/** SHA256 */
#define MBEDTLS_SHA512_C 						/** SHA512 */
#define MBEDTLS_MD_C							/** 开启 消息摘要接口 */

/** mbedtls public key modules */
#define MBEDTLS_PK_C 							/** 开启 公钥密码接口 */
#define MBEDTLS_PK_PARSE_C 						/** 公钥密码解析 */

/** mbedtls RSA modules */
#define MBEDTLS_RSA_C 							/** RSA 算法 */
#define MBEDTLS_PKCS1_V15 						/** 支持 PKCS1_V15 公钥标准 */
#define MBEDTLS_PKCS1_V21 						/** 支持 PKCS1_V21 公钥标准 */

/** mbedtls ECC modules */
#define MBEDTLS_ECP_C 								/** ECC 算法 */
#define MBEDTLS_ECDH_C 								/** ECDH 算法 */
#define MBEDTLS_ECDSA_C 							/** ECDSA 算法 */
#define MBEDTLS_ECP_DP_SECP384R1_ENABLED 			/** 特殊 ECC 参数 */
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED 	/** 开启 ECDHE_ECDSA 密钥协商 */
#define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED 		/** 开启 ECDHE_RSA 密钥协商 */
#define MBEDTLS_KEY_EXCHANGE_PSK_ENABLED            /** 开启 PSK 密钥协商 */

/** mbedtls certificates */
#define MBEDTLS_BASE64_C 							/** base64 编码 */
#define MBEDTLS_CERTS_C 							/** 开启 证书模块 */
#define MBEDTLS_PEM_PARSE_C 						/** pem格式解析 */
#define MBEDTLS_X509_USE_C 							/** x509 接口 */
#define MBEDTLS_X509_CRT_PARSE_C 					/** x509 证书解析 */

/** 自定义套件 */
/** MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256   密钥协商: ECDHE  认证算法: RSA 加密算法: AES_128_GCM 散列算法: SHA256 */
/** MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 密钥协商: ECDHE  认证算法: ECDSA 加密算法: AES_128_GCM 散列算法: SHA256 */
#if 0
#define MBEDTLS_SSL_CIPHERSUITES                         \
		MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,	 \
		MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
#endif

/** 用于检测宏定义正确性 */
#include "mbedtls/check_config.h"

#endif /* STM32F7_MBEDTLS_CONFIG_H */
