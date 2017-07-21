#include <stdio.h>
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

static unsigned char random[64];

int main( void )
{
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    mbedtls_entropy_init( &entropy );
    mbedtls_ctr_drbg_init( &ctr_drbg );

    int ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *) "RANDOM_GEN", 10 );
    if( ret != 0 )
    {
        printf( "failed in mbedtls_ctr_drbg_seed: %d\n", ret );
        goto exit;
    }
    mbedtls_ctr_drbg_set_prediction_resistance( &ctr_drbg, MBEDTLS_CTR_DRBG_PR_OFF );

    ret = mbedtls_ctr_drbg_random( &ctr_drbg, random, sizeof(random) );
    if( ret != 0 )
    {
        printf( "failed in mbedtls_ctr_drbg_random: %d\n", ret );
        goto exit;
    }

    printf("Generating %ld byte random data : ", sizeof(random));
    for(int i = 0; i < sizeof(random); i++)
    {
        printf("%s%02x%s", i % 16 == 0 ? "\n":" ", random[i], i == sizeof(random) - 1 ? "\n":"");
    }

exit:
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );
    
    return 0;
}
