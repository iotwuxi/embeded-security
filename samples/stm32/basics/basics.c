
#include "basics.h"

#define SAMPLE_RANDOM               1
#define SAMPLE_AES                  1
#define SAMPLE_CIPHER_MD            1
#define SAMPLE_RSA_ENC              1
#define SAMPLE_X509                 1
#define SAMPLE_MOD_OPERATE          1

void sample_entry(void)
{
    printf("\nbasics sample start.\n");
    mbedtls_platform_set_calloc_free(platform_calloc, platform_free);
    
#if SAMPLE_CIPHER_MD
    sample_cipher_md();
#endif

#if SAMPLE_X509
    sample_x509();
#endif

#if SAMPLE_AES
    sample_aes();
#endif

#if SAMPLE_RSA_ENC
    sample_rsa_enc();
#endif

#if SAMPLE_MOD_OPERATE
    sample_mod_operate();
#endif
    
#if SAMPLE_RANDOM 
    sample_ctr_random();
    sample_hmac_random();
#endif

    printf("\nbasics sample done.\n");
}
