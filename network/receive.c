#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>

#define PORT 5002

int main()
{
	int sockfd;
	struct sockaddr_in sin;
	if((sockfd=socket(AF_INET, SOCK_DGRAM,0)) < 0)
	{
		perror("socket");
		exit(-1);
	}
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
//	inet_pton(AF_INET, IP, (void *)&sin.sin_addr.s_addr)
 	if(bind(sockfd, (struct sockaddr*)&sin, sizeof(sin)) < 0)
	{
		perror("bind");
			exit(-1);
	}
	char buf[128];
	struct sockaddr_in cin;
	socklen_t addrlen = sizeof(cin);
	while(1)
	{
		bzero(buf, 128);
		if(recvfrom(sockfd, buf, 127, 0,(struct sockaddr*)&cin,&addrlen ) < 0)
		{
			perror("recvfrom");
			exit(-1);
		}
		char ipv4addr[16];
		if(inet_ntop(AF_INET, &cin.sin_addr.s_addr, ipv4addr,sizeof(cin)) == NULL)
		{
			perror("inet_ntop");
			exit(-1);
		}
		printf("recived from %s:%d,data:%s", ipv4addr, ntohs(cin.sin_port) ,buf);
		if(strncmp(buf, "quit", 4) == 0)
			printf("client%s:%d is exiting\n", ipv4addr,ntohs(cin.sin_port));
	}
	close(sockfd);
	return 0;
}
