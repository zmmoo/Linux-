#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#define IP "235.0.1.100"

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

	struct ip_mreq mreq;
	bzero(&mreq, sizeof(mreq));
	mreq.imr_multiaddr.s_addr = inet_addr(IP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(struct ip_mreq));

	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(5002);
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
