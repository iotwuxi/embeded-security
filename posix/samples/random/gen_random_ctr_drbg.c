#include <stdio.h>
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

int main( void )
{
    int i, ret;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    unsigned char buf[64];

    mbedtls_ctr_drbg_init( &ctr_drbg );
    mbedtls_entropy_init( &entropy );
    ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *) "RANDOM_GEN", 10 );
    if( ret != 0 )
    {
        printf( "failed in mbedtls_ctr_drbg_seed: %d\n", ret );
        goto clean;
    }
    mbedtls_ctr_drbg_set_prediction_resistance( &ctr_drbg, MBEDTLS_CTR_DRBG_PR_OFF );

    ret = mbedtls_ctr_drbg_random( &ctr_drbg, buf, sizeof(buf) );
    if( ret != 0 )
    {
        printf("failed!\n");
        goto clean;
    }

    printf( "Generating %ldbyte random data : ", sizeof(buf));
    for(i = 0; i < sizeof(buf); i++)
    {
        if(i%16 == 0)
        {
            printf("\n");
        }
        printf("%02x ", buf[i]);
    }
    printf("\n");

clean:
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );
    
    return 0;
}
