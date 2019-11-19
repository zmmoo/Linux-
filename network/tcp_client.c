#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 5001
#define IP "127.0.0.1"
int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in sin;
	if(argc < 3)
	{
		printf("Usage: %s <ip_addr> <port>\n", argv[0]);
		exit(-1);
	}
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))  < 0)
	{
		perror("socket");
		exit(-1);
	}
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;	
	sin.sin_port = htons(atoi(argv[2])); 
#if 0
	sin.sin_addr.s_addr = inet_addr(IP);
#else
	if(inet_pton(AF_INET, argv[1], (void *)&sin.sin_addr.s_addr) != 1)
	{
		perror("inet_pton");
		exit(-1);
	}
#endif
	if(connect(sockfd, (struct sockaddr *) &sin, sizeof(sin))  < 0)
	{
		perror("connect");
		exit(-1);
	}
	char buf[128];
	bzero(buf, 128);
	while(1)
	{
		fgets(buf, 128 - 1, stdin);
		if(strncmp(buf, "quit", 4) == 0)
			break;
		if(write(sockfd, buf, strlen(buf)) < 0)
			break;

	}
	close(sockfd);	
	return 0; 
}
