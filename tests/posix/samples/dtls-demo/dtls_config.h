#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

/* Debug */
#define MBEDTLS_DEBUG_C

/* System support */
#define MBEDTLS_HAVE_ASM
#define MBEDTLS_HAVE_TIME
#define MBEDTLS_TIMING_C
#define MBEDTLS_NET_C

/* mbed TLS feature support */
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_SRV_C
#define MBEDTLS_SSL_TLS_C
#define MBEDTLS_SSL_COOKIE_C
#define MBEDTLS_SSL_PROTO_DTLS
#define MBEDTLS_SSL_PROTO_TLS1_2
#define MBEDTLS_SSL_DTLS_HELLO_VERIFY

/* mbed bignum modules */
#define MBEDTLS_OID_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C
#define MBEDTLS_BIGNUM_C

/* mbed cipher modules */
#define MBEDTLS_AES_C
#define MBEDTLS_GCM_C
#define MBEDTLS_MD_C
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_CIPHER_C
#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_SHA1_C
#define MBEDTLS_SHA256_C
#define MBEDTLS_SHA512_C

/* mbed public key modules */
#define MBEDTLS_PK_C
#define MBEDTLS_PK_PARSE_C

/* mbed RSA modules */
#define MBEDTLS_RSA_C
#define MBEDTLS_PKCS1_V15
#define MBEDTLS_PKCS1_V21

/* mbed ECC modules */
#define MBEDTLS_ECP_C
#define MBEDTLS_ECDH_C
#define MBEDTLS_ECDSA_C
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
#define MBEDTLS_ECP_DP_SECP384R1_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED

/* For test certificates */
#define MBEDTLS_BASE64_C
#define MBEDTLS_CERTS_C
#define MBEDTLS_PEM_PARSE_C
#define MBEDTLS_X509_USE_C
#define MBEDTLS_X509_CRT_PARSE_C

/* our own ciphersuite list */
#define MBEDTLS_SSL_CIPHERSUITES                         \
		MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,	 \
    	MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256, \
    	MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384	

#include "mbedtls/check_config.h"

#endif /* MBEDTLS_CONFIG_H */