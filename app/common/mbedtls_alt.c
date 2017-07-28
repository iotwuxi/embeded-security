#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#ifdef MBEDTLS_TIMING_ALT

#include "FreeRTOS.h"
#include "task.h"

struct mbedtls_timing_hr_time
{
    unsigned char opaque[4];
};

typedef struct
{
    struct mbedtls_timing_hr_time   timer;
    uint32_t                        int_ms;
    uint32_t                        fin_ms;
} mbedtls_timing_delay_context;

unsigned long mbedtls_timing_get_timer( struct mbedtls_timing_hr_time *val, int reset )
{
    unsigned long delta;
    uint32_t offset;
    uint32_t *t = (uint32_t *) val;

    offset = xTaskGetTickCount();

    if( reset )
    {
        *t = offset;
        return( 0 );
    }

    delta = offset - *t;

    return( delta );
}

/*
 * Set delays to watch
 */
void mbedtls_timing_set_delay( void *data, uint32_t int_ms, uint32_t fin_ms )
{
    mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;

    ctx->int_ms = int_ms;
    ctx->fin_ms = fin_ms;

    if( fin_ms != 0 )
        (void) mbedtls_timing_get_timer( &ctx->timer, 1 );
}

/*
 * Get number of delays expired
 */
int mbedtls_timing_get_delay( void *data )
{
    mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;
    unsigned long elapsed_ms;

    if( ctx->fin_ms == 0 )
        return( -1 );

    elapsed_ms = mbedtls_timing_get_timer( &ctx->timer, 0 );

    if( elapsed_ms >= ctx->fin_ms )
        return( 2 );

    if( elapsed_ms >= ctx->int_ms )
        return( 1 );

    return( 0 );
}

#endif /* MBEDTLS_TIMING_ALT */

#ifdef MBEDTLS_ENTROPY_HARDWARE_ALT

#include <string.h>
#include "stm32f7xx_hal.h"
#include "mbedtls/entropy_poll.h"

extern RNG_HandleTypeDef RngHandle;

int mbedtls_hardware_poll( void *Data, unsigned char *Output, size_t Len, size_t *oLen )
{
    uint32_t index;
    uint32_t randomValue;

    for (index = 0; index < Len/4; index++)
    {
        if (HAL_RNG_GenerateRandomNumber(&RngHandle, &randomValue) == HAL_OK)
        {
            *oLen += 4;
            memset(&(Output[index * 4]), (int)randomValue, 4);
        }
        else
        {
            printf("%s err.\n", __func__);
            while(1);
        }
    }

    return 0;
}

#endif /* MBEDTLS_ENTROPY_HARDWARE_ALT */
