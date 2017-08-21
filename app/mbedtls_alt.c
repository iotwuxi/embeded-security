#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

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
