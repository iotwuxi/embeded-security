#include <stdio.h>
#include "mbedtls/entropy.h"
#include "mbedtls/hmac_drbg.h"

static unsigned char random[64];

void gen_random(void)
{
    mbedtls_entropy_context entropy;
    mbedtls_hmac_drbg_context hmac_drbg;
    const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA1);

    mbedtls_entropy_init(&entropy);
    mbedtls_hmac_drbg_init(&hmac_drbg);

    int ret = mbedtls_hmac_drbg_seed(&hmac_drbg, md_info,
                                     mbedtls_entropy_func, &entropy, (const unsigned char *) "RANDOM_GEN", 10);
    if (ret != 0)
    {
        printf( "failed in mbedtls_hmac_drbg_seed: %d\n", ret );
        goto exit;
    }
    // mbedtls_ctr_drbg_set_prediction_resistance( &ctr_drbg, MBEDTLS_CTR_DRBG_PR_OFF );

    ret = mbedtls_hmac_drbg_random(&hmac_drbg, random, sizeof(random));
    if (ret != 0)
    {
        printf( "failed in mbedtls_ctr_drbg_random: %d\n", ret );
        goto exit;
    }

    printf("Generating %ld byte random data : ", sizeof(random));
    for(int i = 0; i < sizeof(random); i++)
    {
        printf("%s%02X%s", i % 16 == 0 ? "\n":" ", random[i], i == sizeof(random) - 1 ? "\n":"");
    }

exit:
    mbedtls_hmac_drbg_free(&hmac_drbg);
    mbedtls_entropy_free(&entropy);  
}

void sample_entry(void)
{
    gen_random();
}
