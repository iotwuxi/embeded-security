
/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#include <string.h>
#include "udp_server.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdint.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UDP_SERVER_THREAD_PRIO    ( osPriorityAboveNormal )
#define BUFSZ	1024
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief serve tcp connection  
* @param conn: pointer on connection structure 
* @retval None
*/
void udp_server_thread(void* args)
{
    int sock;
    int bytes_read;
    char *recv_data;
    uint32_t addr_len;
    struct sockaddr_in server_addr, client_addr;
    
    /* 分配接收用的数据缓冲 */
    recv_data = malloc(BUFSZ);
    if (recv_data == NULL)
    {
        /* 分配内存失败，返回 */
        printf("No Memory\n");
        return;
    }
    
    /* 创建一个socket，类型是SOCK_DGRAM，UDP类型 */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("Socket Error\n");
        
        /* 释放接收用的数据缓冲 */
        free(recv_data);
        return;
    }
    
    /* 初始化服务端地址 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero),0, sizeof(server_addr.sin_zero));
    
    /* 绑定socket到服务端地址 */
    if (bind(sock,(struct sockaddr *)&server_addr,
             sizeof(struct sockaddr)) == -1)
    {
        /* 绑定地址失败 */
        printf("Bind error\n");
        
        /* 释放接收用的数据缓冲 */
        free(recv_data);
        return;
    }
    
    addr_len = sizeof(struct sockaddr);
    printf("UDP Server Waiting for Client on port 5000...\n");
    
    while (1)
    {
        /* 从sock中收取最大BUFSZ - 1字节数据 */
        bytes_read = recvfrom(sock, recv_data, BUFSZ - 1, 0,
                              (struct sockaddr *)&client_addr, &addr_len);
        /* UDP不同于TCP，它基本不会出现收取的数据失败的情况，除非设置了超时等待 */
        
        recv_data[bytes_read] = '\0'; /* 把末端清零 */
        
        /* 输出接收的数据 */
        printf("\n(%s , %d) said : ",inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        printf("%s", recv_data);
        
        /* 如果接收数据是exit，退出 */
        if (strcmp(recv_data, "exit") == 0)
        {
            lwip_close(sock);
            
            /* 释放接收用的数据缓冲 */
            free(recv_data);
            break;
        }
    }
    
    return;
}

/**
* @brief  Initialize the HTTP server (start its thread) 
* @param  none
* @retval None
*/
void udp_server_init()
{
    sys_thread_new("UDP Server", udp_server_thread, NULL, DEFAULT_THREAD_STACKSIZE, UDP_SERVER_THREAD_PRIO);
}
