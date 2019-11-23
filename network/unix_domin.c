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
#include <sys/un.h>

#define PORT 5001
#define UNIX_PATH "/tmp/my_domain_file.1"
#define IP "127.0.0.1"
void sig_child_handle(int signo)
{
	if(SIGCHLD == signo)
	{
		waitpid(-1, NULL, WNOHANG);
		printf("child is dead\n");
	}
}

int main()
{
	int sockfd;
	int newfd;
	pid_t pid;
	struct sockaddr_un sin;
	signal(SIGCHLD, sig_child_handle);
	if((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
	{
		perror("socket");
		exit(-1);
	}
	int b_reuse;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof(int));
	bzero(&sin, sizeof(sin));
	sin.sun_family = AF_UNIX;
	if(!access(UNIX_PATH, F_OK)) //查看文件是否存在
	{
		unlink(UNIX_PATH);  //删除文件
	}
	strncpy(sin.sun_path, UNIX_PATH, strlen(UNIX_PATH));
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
	while(1)
	{
		if((newfd = accept(sockfd, NULL, NULL)) < 0)

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

