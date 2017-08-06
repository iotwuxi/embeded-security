#include "udp_client.h"

static char buff[MAXSIZE];
static char *msg = "this is udp client\n"; 

void udp_client_thread(void* args)
{
    printf("%s start.\n", __func__);

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
        return;
    }
    
    if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket error\n");
        return;
    }

    if (connect(sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error\n");
        return;
    }   

    for(;;)
    {
        // setsockopt(socket，SOL_SOCKET,SO_SNDTIMEO，(char *)&timeout,sizeof(struct timeval));
        if(write(sock_fd, msg, strlen(msg)) < 0)
        {
            printf("send error\n");
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

void udp_client_init()
{
    sys_thread_new("UDP Client", udp_client_thread, NULL, DEFAULT_THREAD_STACKSIZE, UDP_CLIENT_THREAD_PRIO);
}


void sample_entry(void)
{
    /* 注册任务接口，dhcp成功后开始执行 */
    app_net_register_thread(udp_client_init);

    app_ethernet_init();
}
