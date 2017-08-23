#include "cmsis_os.h"
#include "stm32f7xx_nucleo_144.h"

extern void sample_entry(void);

void bsp_init(void)
{
    /* 初始化MPU(ETH DMA) */
    MPU_Config();

    /* 使能Cache */
    CPU_CACHE_Enable();

    /* 初始化 HAL Liberary */
    HAL_Init();  

    /* 初始化时钟(200MHz) */
    SystemClock_Config(); 

    /* 初始化LED */
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* 初始化熵源 */
    RNG_Init();

    /* 初始化串口 */
    bsp_uart_init(115200); 

    printf("** Hello World - Stm32f767zi-nucleo board. ** \n"); 
}

/**
  * @brief  呼吸灯任务
  */
static void blink_thread(void const * argument)
{
    for( ;; )
    {
        BSP_LED_Toggle(LED2);
        osDelay(1000);
    }
}

/**
  * @brief  启动任务
  */
static void start_thread(void const * argument)
{ 
    /** 示例入口，可以为 `单独测试接口` 或 `任务接口`. */
    sample_entry();

    for( ;; )
    {
        /* 删除启动任务 */ 
        osThreadTerminate(NULL);
    }
}

int main(void)
{
    /** 硬件接口初始化 */
    bsp_init();

    /* 初始化启动任务 */
    osThreadDef(start, start_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 5);
    osThreadCreate (osThread(start), NULL);

    /* 呼吸灯任务创建 */
    osThreadDef(blink, blink_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
    osThreadCreate (osThread(blink), NULL);

    /* 启动任务调度 */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    for( ;; );
}

