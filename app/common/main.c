#include "cmsis_os.h"
#include "stm32f7xx_nucleo_144.h"

extern void bsp_init(void);
extern void app_init(void);

/**
  * @brief  呼吸灯任务
  */
static void BlinkThread(void const * argument)
{
    for( ;; )
    {
        BSP_LED_Toggle(LED2);
        osDelay(1000);
    }
}

int main(void)
{
	/** 硬件接口初始化 */
    bsp_init();

	/** 任务接口初始化 */
    app_init();
    
    /* 呼吸灯任务创建 */
    osThreadDef(Blink, BlinkThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
    osThreadCreate (osThread(Blink), NULL);

    /* 启动任务调度 */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    for( ;; );
}