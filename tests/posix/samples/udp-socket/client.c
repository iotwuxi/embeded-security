#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <arpa/inet.h>	

#include <unistd.h>				// sleep

#define MAXSIZE  				2048
#define SERV_PORT				5000

static char buff[MAXSIZE];
static char *msg = "this is udp client\n"; 

void main(int argc , char **argv)
{
	int sock_fd, n;
	struct sockaddr_in servaddr;
	struct timeval timeout = 
	{
		.tv_sec = 3,
		.tv_usec = 0
	};

	if(argc != 2)
	{
		printf("usage: client <IPaddress>\n");
		return;
	}

	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);

	if((inet_pton(AF_INET , argv[1], &servaddr.sin_addr)) <= 0)
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
	    if(send(sock_fd, msg, strlen(msg), 0) < 0)
	    {
	    	printf("send error\n");
	    	return;
	    }

	    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
		if( ( n = recv(sock_fd, buff, MAXSIZE, 0)) < 0)
		{
			printf("recv error\n");
		}
		else
		{
			buff[n] = '\0';
			printf("receive data: %s\n", buff);
		}

		sleep(3);
	}
} 