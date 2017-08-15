#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <arpa/inet.h>          // socket
#include <unistd.h>             // sleep

#include "coap.h"

#define SERV_ADDR               "139.196.187.107" 
#define SERV_PORT               5683

#define COAP_GET_URL            "test"
#define COAP_POST_URL           "test"

#define MSG_BUF_LEN             256     
#define MAXSIZE                 2048

static int sock_fd;
static uint8_t msg_send_buf[MSG_BUF_LEN];			
static coap_pdu msg_send = {msg_send_buf, 0, MSG_BUF_LEN};
static uint8_t msg_recv_buf[MSG_BUF_LEN];          
static coap_pdu msg_recv = {msg_recv_buf, 0, MSG_BUF_LEN};

static struct timeval timeout = 
{
    .tv_sec = 2,
    .tv_usec = 0
};

static void coap_net_init(void)
{
    struct sockaddr_in servaddr;

    bzero(&servaddr , sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);

    if((inet_pton(AF_INET , SERV_ADDR, &servaddr.sin_addr)) <= 0)
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
}

void coap_client_get(void)
{
    coap_init_pdu(&msg_send);                            
    coap_set_version(&msg_send, COAP_V1);                
    coap_set_type(&msg_send, CT_CON);                    
    coap_set_token(&msg_send, 0x3412, 2);                
    coap_set_code(&msg_send, CC_GET);                    
    coap_set_mid(&msg_send, 1234);                      
    coap_add_option(&msg_send, CON_URI_PATH, COAP_GET_URL, strlen(COAP_GET_URL)); 

    if(send(sock_fd, msg_send.buf, msg_send.len, 0) < 0)
    {
        printf("send error\n");
        return;
    }

    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
    if( ( msg_recv.len = recv(sock_fd, msg_recv.buf, MAXSIZE, 0)) < 0)
    {
        printf("recv error\n");
        return;
    }
    
    if(coap_validate_pkt(&msg_recv) != CE_INVALID_PACKET)
    {
        if(coap_get_code(&msg_recv) == CC_CONTENT)
        {
            coap_payload pyload = coap_get_payload(&msg_recv);
            printf("\n[GET] coap:%s:%d/%s pyload:\n%s\n", SERV_ADDR, SERV_PORT, COAP_GET_URL, pyload.val);
        }
        else
        {
            printf("coap pkt code error[%d]\n", coap_get_code(&msg_recv));
        }
    }
    else
    {
        printf("coap pkt error\n");
    }
}


void coap_client_post(void)
{
    uint8_t *coap_post_msg = "xxxxxx.";

    coap_init_pdu(&msg_send);                            
    coap_set_version(&msg_send, COAP_V1);                
    coap_set_type(&msg_send, CT_CON);                    
    coap_set_token(&msg_send, 0x7856, 2);                
    coap_set_code(&msg_send, CC_POST);                   
    coap_set_mid(&msg_send, 6789);                      
    coap_add_option(&msg_send, CON_URI_PATH, COAP_POST_URL, strlen(COAP_POST_URL));
    coap_set_payload(&msg_send, coap_post_msg, strlen(coap_post_msg)); 

    if(send(sock_fd, msg_send.buf, msg_send.len, 0) < 0)
    {
        printf("send error\n");
        return;
    }

    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
    if( ( msg_recv.len = recv(sock_fd, msg_recv.buf, MAXSIZE, 0)) < 0)
    {
        printf("recv error\n");
        return;
    }
    
    if(coap_validate_pkt(&msg_recv) != CE_INVALID_PACKET)
    {
        if(coap_get_code(&msg_recv) == CC_CREATED)
        {
            coap_payload pyload = coap_get_payload(&msg_recv);
            printf("[POST] coap:%s:%d/%s pyload: %s [success.]\n", SERV_ADDR, SERV_PORT, COAP_POST_URL, pyload.val);
        }
        else
        {
            printf("coap pkt code error[%d]\n", coap_get_code(&msg_recv));
        }
    }
    else
    {
        printf("coap pkt error\n");
    } 

}

void main(void)
{
    coap_net_init();

    for(;;)
    {
        coap_client_get();
        coap_client_post();
        sleep(3);       
    }
}
