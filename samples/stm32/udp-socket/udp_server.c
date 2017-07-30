#include "udp_server.h"

/**
* @brief UDP服务器任务
*/
void udp_server_thread(void* args)
{
    printf("%s start.\n", __func__);
    
    int sock;
    int len;
    char *recv_data;
    uint32_t addr_len;
    struct sockaddr_in server_addr, client_addr;

    struct timeval tv;
    int maxfd = 0;
    int ret = 0;
    fd_set read_fds;
    
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
        FD_ZERO(&read_fds);
        FD_SET(sock, &read_fds);
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        maxfd = sock;

        ret = select(maxfd + 1, &read_fds, NULL, NULL, &tv);
        
        if (ret == 0) 
        {
            continue;
        }

        if (ret > 0) 
        {
            if (FD_ISSET(sock, &read_fds)) 
            {
                len = recvfrom(sock, recv_data, BUF_SIZE - 1, 0,
                  (struct sockaddr *)&client_addr, &addr_len);
        
                recv_data[len] = '\0';
        
                printf("\n(%s , %d) said : ", inet_ntoa(client_addr.sin_addr),
                    ntohs(client_addr.sin_port));
                printf("%s", recv_data);

                // echo
                sendto(sock, recv_data, strlen(recv_data), 0,
                    (struct sockaddr *)&client_addr, sizeof(struct sockaddr));
            }
        }       
    }
    
    closesocket(sock);
    free(recv_data);
    return;
}

#if 0
void udp_server_thread(void* args)
{
    int sock;
    int len;
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
        len = recvfrom(sock, recv_data, BUF_SIZE - 1, 0,
                              (struct sockaddr *)&client_addr, &addr_len);
        
        recv_data[len] = '\0';
        
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

void app_init(void)
{
    /* 注册任务接口，dhcp成功后开始执行 */
    app_net_register_thread(udp_server_init);

    app_ethernet_init();
}
