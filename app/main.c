#include "cmsis_os.h"
#include "stm32f7xx_nucleo_144.h"

extern void bsp_init(void);
extern void app_init(void);

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
    /** 任务接口初始化 */
    app_init();

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

