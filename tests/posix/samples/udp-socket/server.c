#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXSIZE 			2048
#define SERV_PORT 			5000
static char buff[MAXSIZE];

void main(int argc , char **argv)
{
	int n, sock_fd;

	struct sockaddr_in server_addr, client_addr;

	bzero(&server_addr , sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERV_PORT);

	if((sock_fd = socket(AF_INET , SOCK_DGRAM , 0)) < 0)
	{
		printf("socket error");
		return;
	}

    if (bind(sock_fd,(struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        printf("bind error\n");
        return;
    }

    uint32_t addr_len = sizeof(struct sockaddr);
    printf("udp server waiting for client on port %d...\n", SERV_PORT);

	for(;;)
	{
		if((n = recvfrom(sock_fd, buff, MAXSIZE, 0, (struct sockaddr *)&client_addr , &addr_len))<0)
		{
			printf("recvfrom error\n");
			return;
		}

        printf("\n(%s:%d)said: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buff);

		if((n = sendto(sock_fd, buff, n, 0, (struct sockaddr *)&client_addr, addr_len)) < 0)
		{
			printf("sendto error\n");
			return;
		}
	}
}