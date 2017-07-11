
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#include <string.h>
#include "udp_server.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdint.h>

#define UDP_SERVER_THREAD_PRIO      (osPriorityAboveNormal)
#define BUF_SIZE	                1024
#define PORT                        5000

/**
* @brief UDP服务器任务
*/
void udp_server_thread(void* args)
{
    int sock;
    int bytes_read;
    char *recv_data;
    uint32_t addr_len;
    struct sockaddr_in server_addr, client_addr;

    struct timeval tv;
    int maxfd = 0;
    int retval = 0;
    fd_set readfds;
    
    recv_data = malloc(BUF_SIZE);
    if (recv_data == NULL) {
        printf("no memory\n");
        return;
    }
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("socket error\n");
        free(recv_data);
        return;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero),0, sizeof(server_addr.sin_zero));
    
    if (bind(sock,(struct sockaddr *)&server_addr,
             sizeof(struct sockaddr)) == -1)
    {
        printf("bind error\n");
        free(recv_data);
        return;
    }
    
    addr_len = sizeof(struct sockaddr);
    printf("udp server waiting for client on port %d...\n", PORT);
    // listen(sock, 10);
    
    while (1)
    {

        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        maxfd = sock;

        retval = select(maxfd + 1, &readfds, NULL, NULL, &tv);

        if (retval > 0) 
        {
            if (FD_ISSET(sock, &readfds)) 
            {
                bytes_read = recvfrom(sock, recv_data, BUF_SIZE - 1, 0,
                  (struct sockaddr *)&client_addr, &addr_len);
        
                recv_data[bytes_read] = '\0';
        
                printf("\n(%s , %d) said : ", inet_ntoa(client_addr.sin_addr),
                    ntohs(client_addr.sin_port));
                printf("%s", recv_data);

                if (strcmp(recv_data, "exit") == 0)
                {
                    closesocket(sock);
                    free(recv_data);
                    break;
                }
            }
        }       
    }
    
    return;
}

#if 0
void udp_server_thread(void* args)
{
    int sock;
    int bytes_read;
    char *recv_data;
    uint32_t addr_len;
    struct sockaddr_in server_addr, client_addr;
    
    recv_data = malloc(BUF_SIZE);
    if (recv_data == NULL) {
        printf("no memory\n");
        return;
    }
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("socket error\n");
        free(recv_data);
        return;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero),0, sizeof(server_addr.sin_zero));
    
    if (bind(sock,(struct sockaddr *)&server_addr,
             sizeof(struct sockaddr)) == -1)
    {
        printf("bind error\n");
        free(recv_data);
        return;
    }
    
    addr_len = sizeof(struct sockaddr);
    printf("udp server waiting for client on port %d...\n", PORT);
    
    while (1)
    {
        bytes_read = recvfrom(sock, recv_data, BUF_SIZE - 1, 0,
                              (struct sockaddr *)&client_addr, &addr_len);
        
        recv_data[bytes_read] = '\0';
        
        printf("\n(%s , %d) said : ", inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        printf("%s", recv_data);
        
        if (strcmp(recv_data, "exit") == 0)
        {
            closesocket(sock);
            free(recv_data);
            break;
        }
    }
    
    return;
}
#endif

/**
* @brief  初始化UDP服务器
*/
void udp_server_init()
{
    sys_thread_new("UDP Server", udp_server_thread, NULL, DEFAULT_THREAD_STACKSIZE, UDP_SERVER_THREAD_PRIO);
}
