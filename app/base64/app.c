#include <stdio.h>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"

#include "cmsis_os.h"

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif
#include "mbedtls/base64.h"

#define mbedtls_printf     printf

static const unsigned char base64_test_dec[64] =
{
    0x24, 0x48, 0x6E, 0x56, 0x87, 0x62, 0x5A, 0xBD,
    0xBF, 0x17, 0xD9, 0xA2, 0xC4, 0x17, 0x1A, 0x01,
    0x94, 0xED, 0x8F, 0x1E, 0x11, 0xB3, 0xD7, 0x09,
    0x0C, 0xB6, 0xE9, 0x10, 0x6F, 0x22, 0xEE, 0x13,
    0xCA, 0xB3, 0x07, 0x05, 0x76, 0xC9, 0xFA, 0x31,
    0x6C, 0x08, 0x34, 0xFF, 0x8D, 0xC2, 0x6C, 0x38,
    0x00, 0x43, 0xE9, 0x54, 0x97, 0xAF, 0x50, 0x4B,
    0xD1, 0x41, 0xBA, 0x95, 0x31, 0x5A, 0x0B, 0x97
};

static void BlinkThread(void const * argument);
int base64_test(void);

int main(void)
{
    /* 初始化 HAL Liberary */
    HAL_Init();  

    /* 初始化时钟(200MHz) */
    SystemClock_Config(); 

    /* 初始化LED */
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* 初始化串口 */
    bsp_uart_init(115200);

    printf("Base64 Example\n");

    /* 初始化呼吸灯任务 */
    osThreadDef(Blink, BlinkThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);

    /* 创建呼吸灯任务 */
    osThreadCreate (osThread(Blink), NULL);

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    for( ;; );
}

/**
  * @brief  呼吸灯任务
  */
static void BlinkThread(void const * argument)
{
    base64_test();
    for( ;; )
    {
        BSP_LED_Toggle(LED2);
        // printf("blink task.\n");
        osDelay(1000);
    }
}

/**
  * @brief  SysTick_Handler
  */
void SysTick_Handler(void)
{   
    osSystickHandler();
}

int base64_test(void)
{
    unsigned char buffer[128];
    size_t len;

    if (mbedtls_base64_encode(buffer, sizeof(buffer), &len, base64_test_dec, 64) != 0 )
    {
        mbedtls_printf("base64 failed\n");
        return 1;
    }

    buffer[len] = '\0';
    mbedtls_printf("base64 encode length: %d\n", len);
    mbedtls_printf("base64 encode result: %s\n", buffer);

    return 0;
}