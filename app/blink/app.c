#include "stdio.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"

#include "cmsis_os.h"

static void BlinkThread(void const * argument);

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

    printf("** Hello World - Stm32f767zi-nucleo board. ** \n");

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
    for( ;; )
    {
        BSP_LED_Toggle(LED2);
        printf("blink task.\n");
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