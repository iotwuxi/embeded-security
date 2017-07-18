#include "main.h"
#include "cmsis_os.h"
#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "app_ethernet.h"
#include "dtls_client.h"

static void RNG_Init(void);
static void BlinkThread(void const * argument);

RNG_HandleTypeDef RngHandle;

int main(void)
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

    RNG_Init();

    /* 初始化串口 */
    bsp_uart_init(115200);

    printf("** Hello World - Stm32f767zi-nucleo board. ** \n");
    
    dtls_client_init();
    
    /* 初始化呼吸灯任务 */
    osThreadDef(Blink, BlinkThread, osPriorityRealtime, 0, configMINIMAL_STACK_SIZE * 2);
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
        osDelay(1000);
    }
}

/* RNG init function */
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
