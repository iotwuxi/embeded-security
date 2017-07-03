#include "main.h"
#include "cmsis_os.h"
#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "app_ethernet.h"
#include "httpserver-netconn.h"

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif 

static struct netif gnetif;
static UART_HandleTypeDef UartHandle;
static uart_init(uint32_t baudrate);
static void SystemClock_Config(void);
static void StartThread(void const * argument);
static void Netif_Config(void);
static void Error_Handler(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void BlinkThread(void const * argument);

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

    /* 初始化串口 */
    uart_init(115200);

    printf("** Hello World - Stm32f767zi-nucleo board. ** \n");

    /* 初始化启动任务 */
#if defined(__GNUC__)
    osThreadDef(Start, StartThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 5);
#else
    osThreadDef(Start, StartThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
#endif
    /* 初始化呼吸灯任务 */
    osThreadDef(Blink, BlinkThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);

    osThreadCreate (osThread(Blink), NULL);
    osThreadCreate (osThread(Start), NULL);

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    for( ;; );
}

static uart_init(uint32_t baudrate)
{
    UartHandle.Instance        = USARTx;
    UartHandle.Init.BaudRate   = baudrate;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief  printf 重定向到 USART3(开发板虚拟串口)
  */
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}

/**
  * @brief  启动任务
  */
static void StartThread(void const * argument)
{ 
    /* 创建 tcp_ip 任务 */
    tcpip_init(NULL, NULL);

    /* 初始化 Lwip 协议栈 */
    Netif_Config();

    /* 初始化 http_server 任务 */
    http_server_netconn_init();

    /* 网络状态指示(LED) */
    User_notification(&gnetif);

#ifdef USE_DHCP
    /* 开启 DHCP 任务 */
    osThreadDef(DHCP, DHCP_thread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE * 2);
    osThreadCreate (osThread(DHCP), &gnetif);
#endif

    for( ;; )
    {
        /* 删除初始化任务 */ 
        osThreadTerminate(NULL);
    }
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
  * @brief  初始化 Lwip 协议栈
  */
static void Netif_Config(void)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;

#ifdef USE_DHCP
    ip_addr_set_zero_ip4(&ipaddr);
    ip_addr_set_zero_ip4(&netmask);
    ip_addr_set_zero_ip4(&gw);
#else
    IP_ADDR4(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
    IP_ADDR4(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
    IP_ADDR4(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif /* USE_DHCP */

    /* add the network interface */    
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

    /*  Registers the default network interface. */
    netif_set_default(&gnetif);

    if (netif_is_link_up(&gnetif))
    {
        /* When the netif is fully configured this function must be called.*/
        netif_set_up(&gnetif);
    }
    else
    {
        /* When the netif link is down this function must be called */
        netif_set_down(&gnetif);
    }
}

/**
  * @brief  初始化系统时钟(200MHz)
  */
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    HAL_StatusTypeDef ret = HAL_OK;

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 400;  
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 9;
    RCC_OscInitStruct.PLL.PLLR = 7;  

    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if(ret != HAL_OK)
    {
        Error_Handler();
    }

    ret = HAL_PWREx_EnableOverDrive();
    if(ret != HAL_OK)
    {
        Error_Handler();
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2; 

    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
    if(ret != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief  异常指示
  */
static void Error_Handler(void)
{
    printf("%s\n", __func__);
    while(1)
    {
        ;
    }
}

/**
  * @brief  Configure the MPU attributes as Device for  Ethernet Descriptors in the SRAM1.
  * @note   The Base Address is 0x20020000 since this memory interface is the AXI.
  *         The Configured Region Size is 256B (size of Rx and Tx ETH descriptors) 
  *       
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;

    /* Disable the MPU */
    HAL_MPU_Disable();

    /* Configure the MPU attributes as Device for Ethernet Descriptors in the SRAM */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x20020000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Enable the MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
    /* Enable I-Cache */
    SCB_EnableICache();

    /* Enable D-Cache */
    SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  assert_failed
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
    while (1)
    {
        ;
    }
}
#endif
