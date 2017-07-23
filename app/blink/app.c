#include "stdio.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"

#include "cmsis_os.h"

RNG_HandleTypeDef RngHandle;
static void BlinkThread(void const * argument);
static void RNG_Init(void);

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
    
    RNG_Init();

    printf("Hello World **Stm32f767zi-nucleo board**\n");

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
    uint32_t random = 0;
    for( ;; )
    {
        BSP_LED_Toggle(LED2);
        osDelay(1000);
        
        if (HAL_RNG_GenerateRandomNumber(&RngHandle, &random) != HAL_OK)
        {
            /* Random number generation error */
            Error_Handler();
        }
        printf("Random Value: %08lX\n", random);
    }
}

/**
  * @brief  SysTick_Handler
  */
void SysTick_Handler(void)
{   
    osSystickHandler();
}

/**
  * @brief 初始化随机数生成器
  */
static void RNG_Init(void)
{
  RngHandle.Instance = RNG;
 /* DeInitialize the RNG peripheral */
  if (HAL_RNG_DeInit(&RngHandle) != HAL_OK)
  {
    /* DeInitialization Error */
    Error_Handler();
  }    

  /* Initialize the RNG peripheral */
  if (HAL_RNG_Init(&RngHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}