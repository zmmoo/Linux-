#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#define PORT 5001
#define IP "127.0.0.1"
void *cli_data_handle(void *arg);
int main()
{
	int sockfd;
	int newfd;
	struct sockaddr_in sin;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))  < 0)
	{
		perror("socket");
		exit(-1);
	}
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;	
	sin.sin_port = htons(PORT); 
#if 0
	sin.sin_addr.s_addr = inet_addr(IP);
	// sin.sin_addr.s_addr = htonl(INADDR_ANY); 服务器可以绑定到任意机器 从任意网卡过的数据都能处理
#else
	if(inet_pton(AF_INET, IP, (void *)&sin.sin_addr.s_addr) != 1)
	{
		perror("inet_pton");
		exit(-1);
	}
#endif
	if(bind(sockfd, (struct sockaddr *) &sin, sizeof(sin))  < 0)
	{
		perror("bind");
		exit(-1);
	}
	if(listen(sockfd, 5) < 0)
	{
		perror("listen");
		exit(-1);
	}
	int b_reuse;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof(int));
#if 0

	struct sockaddr_in cin;
	socklen_t addrlen = sizeof(cin);
	if((new_fd = accept(sockfd, (struct sockaddr*)&cin, &addrlen)) < 0)
	{
		perror("accept");
		exit(-1);
	}
	char ipv4addr[16];
	if(inet_ntop(AF_INET, (void *)&cin.sin_addr.s_addr, ipv4addr, sizeof(cin)) == NULL)	
	{
		perror("inet_ntop");
		exit(-1);
	}
	printf("client:%s:%d is connected\n", ipv4addr, ntohs(cin.sin_port));
	char buf[128];
	bzero(buf, 128);
	while(read(new_fd, buf, 128) > 0)
	{
		if(strncmp(buf, "quit", 4) == 0)
			break;
		printf("%s", buf);
	}
#else
	pthread_t tid;
	while(1)
	{
		struct sockaddr_in cin;
		socklen_t addrlen = sizeof(cin);
		if((newfd = accept(sockfd, (struct sockaddr*)&cin, &addrlen)) < 0)
		{
			perror("accept");
			exit(-1);
		}
		char ipv4addr[16];
		if(inet_ntop(AF_INET, (void *)&cin.sin_addr.s_addr, ipv4addr, sizeof(cin)) == NULL)	
		{
			perror("inet_ntop");
			exit(-1);
		}
		printf("client:%s:%d is connected\n", ipv4addr, ntohs(cin.sin_port));

		pthread_create(&tid, NULL, cli_data_handle, (void *)&newfd);
	}
#endif
	close(sockfd);	
	return 0; 
}

void *cli_data_handle(void *arg)
{
	int newfd = *(int*)arg;
	printf("handler thread:newfd = %d\n", newfd);

	char buf[128];
	bzero(buf, 128);
	while(read(newfd, buf, 128) > 0)
	{
		if(strncmp(buf, "quit", 4) == 0)
			break;
		printf("%s", buf);
		bzero(buf, 128);
	}
}
