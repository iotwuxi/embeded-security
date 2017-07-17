#include "main.h"
#include "cmsis_os.h"
#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "app_ethernet.h"
#include "dtls_client.h"

static struct netif gnetif;
static void Netif_Config(void);
static void BlinkThread(void const * argument);
static void StartThread(void const * argument);

static void RNG_Init(void);
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

/**
  * @brief  启动任务
  */
static void StartThread(void const * argument)
{ 
    /* 创建 tcp_ip 任务 */
    tcpip_init(NULL, NULL);

    /* 初始化 Lwip 协议栈 */
    Netif_Config();

    dtls_client_init();

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
