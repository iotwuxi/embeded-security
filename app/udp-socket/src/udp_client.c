#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#include <string.h>
#include "udp_server.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdint.h>

#define UDP_CLIENT_THREAD_PRIO      (osPriorityAboveNormal)
#define SERVER_IP                   "192.168.0.9"
#define PORT                        5000

const char send_data[] = "this is udp client\n"; 

void udp_client_thread(void* args)
{
    int sock;
    // struct hostent *host;
    struct sockaddr_in server_addr;
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket error\n");
        return;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    sendto(sock, send_data, strlen(send_data), 0,
        (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

    closesocket(sock);
}

/**
* @brief  初始化UDP客户端
*/
void udp_client_init()
{
    sys_thread_new("UDP Server", udp_client_thread, NULL, DEFAULT_THREAD_STACKSIZE, UDP_CLIENT_THREAD_PRIO);
}