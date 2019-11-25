#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>


#define N 32
#define R 1 // user register
#define L 2 //user login
#define Q 3 //user query
#define H 4 //user history

typedef struct{
	int type;
	char name[N];
	char data[2048];
}MSG;

int do_register(int sockfd, MSG *msg)
{
	msg->type = R;
	printf("Input name:");
	scanf("%s", msg->name);

	printf("Input passwd:");	
	scanf("%s",msg->data);

	if(send(sockfd, msg, sizeof(MSG), 0) < 0)
	{
		perror("send");
		exit(-1);
	}
	printf("client send ok\n");
	if(recv(sockfd, msg, sizeof(MSG), 0) < 0)
	{
		perror("recv");
		exit(-1);
	}
	printf("%s\n", msg->data);
	return 0;
}

int do_login(int sockfd, MSG *msg)
{
	msg->type = L;

	printf("Input name:");
	scanf("%s", msg->name);
	getchar();

	printf("Input passwd:");
	scanf("%s", msg->data);
	getchar();

	if(send(sockfd, msg, sizeof(MSG), 0)  < 0)
	{
		perror("send");
		return 0;
	}
	if(recv(sockfd, msg, sizeof(MSG), 0) < 0)
	{
		perror("recv");
		return 0;
	}
	if(strncmp(msg->data, "OK", 3) == 0)
	{

		printf("login OK\n");
		return 1;
	}
	else
	{
		printf("%s\n", msg->data);
	}
	return 0;
}

int do_query(int sockfd, MSG *msg)
{
	msg->type = Q;
	
	puts("________________\n");
	while(1)
	{
		printf("Input word\n");
		scanf("%s", msg->data);
		getchar();
		if(strncmp(msg->data, "#", 1) == 0)
		{//　# 返回上一级菜单
			break;
		}
		if(send(sockfd, msg, sizeof(MSG), 0) < 0)
		{
			perror("send");
			exit(-1);
		}
		if(recv(sockfd, msg, sizeof(MSG), 0) < 0)
		{
			perror("recv");
			exit(-1);
		}
		printf("%s\n", msg->data);
	}
	return 0;
}

int do_history(int sockfd, MSG *msg)
{
	msg->type = H;
	printf("you had rearch word\n");
	if(send(sockfd, msg, sizeof(MSG), 0) < 0)
	{
		perror("send");
		exit(-1);
	}
	if(recv(sockfd, msg, sizeof(MSG), 0) < 0)
	{
		perror("recv");
		exit(-1);
	}
	printf("%s\n", msg->data);
	return 0;
}


int main(int argc, char *argv[])
{
	int sockfd;
	pid_t pid;
	int cmd;
	struct sockaddr_in serveraddr;
	MSG *msg;
	if(argc != 3)
	{
		printf("Usage:%s <server_ip> <port>\n", argv[0]);
		exit(-1);
	}
	if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("sockfd");
		exit(-1);
	}
	bzero(&serveraddr, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));
	if(connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr)) < 0)
	{
		perror("connect");
		exit(-1);
	}
	while(1)
	{
		printf("\n***********************************\n");
		printf("1 register 2 login 3 quit\n");
		printf("\n****l*******************************\n");
		scanf("%d", &cmd);
		getchar();
		switch(cmd)
		{
			case R:
				do_register(sockfd, msg);
				break;
			case L:
				if(do_login(sockfd, msg) == 1)
				{
					goto next;
				}
				break;
			case 3:
				close(sockfd);
				exit(0);
				break;
			default:
				printf("Invaild error\n");
		}
	}
next:
	while(1)
	{
		
		printf("\n***********************************\n");
		printf("1 query 2 history 3 quit\n");
		printf("\n****l*******************************\n");
		scanf("%d", &cmd);
		getchar();
		switch(cmd)
		{
			case 1:
				do_query(sockfd, msg);
				break;
			case 2:
				do_history(sockfd, msg);
				break;
			case 3:
				close(sockfd);
				exit(-1);
			default:
				printf("Invaild cmd\n");

		}

	}
	return 0;
}
