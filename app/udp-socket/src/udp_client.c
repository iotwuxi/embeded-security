#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#include <string.h>
#include "udp_server.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdint.h>

#define UDP_CLIENT_THREAD_PRIO      (osPriorityNormal)

/** netcat -l -u 139.196.187.107 -p 5000 & */
//#define SERVER_IP                   "139.196.187.107"
//#define PORT                        5000

#define SERVER_IP                   "139.196.187.107"
#define MAXSIZE                     256
#define SERV_PORT                   5000

static char buff[MAXSIZE];
static char *msg = "this is udp client\n"; 

void thread_err_handle(void);

void udp_client_thread(void* args)
{
    int sock_fd, n;
    struct sockaddr_in servaddr;
#if 0    
    struct timeval timeout = 
    {
        .tv_sec = 3,
        .tv_usec = 0
    };
#endif
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if((inet_pton(AF_INET , SERVER_IP, &servaddr.sin_addr)) <= 0)
    {
        printf("inet_pton error\n");
        thread_err_handle();
    }
    
    if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket error\n");
        thread_err_handle();
    }

    if (connect(sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error\n");
        thread_err_handle();
    }   

    for(;;)
    {
        // setsockopt(socket，SOL_SOCKET,SO_SNDTIMEO，(char *)&timeout,sizeof(struct timeval));
        if(write(sock_fd, msg, strlen(msg)) < 0)
        {
            printf("send error\n");
            thread_err_handle();
        }

        // setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
        if( ( n = read(sock_fd, buff, MAXSIZE)) < 0)
        {
            printf("recv error\n");
        }
        else
        {
            buff[n] = '\0';
            printf("receive data: %s\n", buff);
        }

        osDelay(2000);
    }   
}

/**
* @brief  初始化UDP客户端
*/
void udp_client_init()
{
    sys_thread_new("UDP Server", udp_client_thread, NULL, DEFAULT_THREAD_STACKSIZE, UDP_CLIENT_THREAD_PRIO);
}

void thread_err_handle(void)
{
    while(1)
    {

    }
}