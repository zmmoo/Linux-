#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#define IP "127.0.0.255"

int main()
{
	int sockfd;
	struct sockaddr_in sin;
	socklen_t sockaddr = sizeof(sin);
	if((sockfd=socket(AF_INET,SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		exit(-1);
	}		
	int b_br = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &b_br,sizeof(int));

	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(5002);
#if 0
	if(inet_pton(AF_INET,IP, (void *)&sin.sin_addr.s_addr) != 1)	
	{
		perror("inet_pton");
		exit(-1);
	}
#else
	sin.sin_addr.s_addr = inet_addr(IP);
#endif
	char buf[128];
	while(1)
	{
		bzero(buf, 128);
		fgets(buf, 127, stdin);
		sendto(sockfd, buf ,128, 0, (struct sockaddr*)&sin, sizeof(sin));
		if(strncmp(buf, "quit", 4) == 0)
		{
			break;
		}
	}
	close(sockfd);

}
