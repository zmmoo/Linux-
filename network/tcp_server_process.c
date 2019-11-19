#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>

#define PORT 5001
#define IP "127.0.0.1"
void sig_child_handle(int signo)
{
	if(SIGCHLD == signo)
	{
		waitpid(-1, NULL, WNOHANG);
	}
}

int main()
{
	int sockfd;
	int newfd;
	pid_t pid;
	struct sockaddr_in sin;
	signal(SIGCHLD, sig_child_handle);
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
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
	struct sockaddr_in cin;
	socklen_t addrlen = sizeof(cin);
	while(1)
	{
		if((newfd = accept(sockfd, (struct sockaddr*)&cin, &addrlen)) < 0)
		{
			perror("accept");
			exit(-1);
		}
		if((pid = fork()) < 0)
		{
			perror("fork");
			break;
		}
		if(pid == 0)
		{
			close(sockfd);

			char ipv4addr[16];
			if(inet_ntop(AF_INET, (void *)&cin.sin_addr.s_addr, ipv4addr, sizeof(cin)) == NULL)	
			{
				perror("inet_ntop");
				exit(-1);
			}
			printf("client:%s:%d is connected\n", ipv4addr, ntohs(cin.sin_port));
			char buf[128];
			bzero(buf, 128);
			while(read(newfd, buf, 128) > 0)
			{
				if(strncmp(buf, "quit", 4) == 0)
					break;
				printf("%s", buf);
				bzero(buf, 128);
			}
			close(newfd);
			return 0;
		}
		else
		{
			close(newfd);
		}
	}

	close(sockfd);	
	return 0; 
}

