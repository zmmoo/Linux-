#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>


#define N 32
#define R 1  // user register
#define L 2 //user login
#define Q 3 //user query
#define H 4 //user history
#define DATABASE "my.db"

typedef struct{
	int type;
	char name[N];
	char data[2048];
}MSG;

int do_client(int acceptfd, sqlite3 *db);
void do_register(int acceptfd, MSG *msg, sqlite3 *db);
void do_login(int acceptfd, MSG *msg, sqlite3 *db);
void do_query(int acceptfd, MSG *msg, sqlite3 *db);
void do_history(int acceptfd, MSG *msg, sqlite3 *db);

int main(int argc, char *argv[])
{
	int sockfd;
	int acceptfd;
	pid_t pid;
	struct sockaddr_in serveraddr;
	MSG msg;
	sqlite3 *db;

	if(argc != 3)
	{
		printf("Usage:%s <server_ip> <port>\n", argv[0]);
		exit(-1);
	}

	signal(SIGCHLD,SIG_IGN);

	if(sqlite3_open(DATABASE, &db) != SQLITE_OK)
	{
		printf("%s\n", sqlite3_errmsg(db));
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
	if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr)) < 0)
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
		if((acceptfd=accept(sockfd, NULL, NULL)) < 0)
		{
			perror("accept");
			exit(-1);
		}
		if((pid = fork()) < 0)
		{
			perror("fork");
			exit(-1);
		}
		else if(pid == 0)	
		{ //处理客户端
			printf("this is child\n");
			do_client(acceptfd, db);

		}
		else
		{ //父进程
			close(acceptfd);
		}

	}
	return 0;
}

int do_client(int acceptfd, sqlite3 *db)
{
	MSG msg;
	printf("recving\n");
	while(read(acceptfd, &msg, sizeof(MSG)) > 0)
	{
		printf("recved\n");
		printf("%d\n",msg.type);
		switch(msg.type)	
		{
			case R:
				do_register(acceptfd, &msg, db);				
				break;
			case L:
				do_login(acceptfd, &msg, db);
				break;
			case Q:
				do_query(acceptfd, &msg,  db);
				break;
			case H:
				do_history(acceptfd, &msg, db);
				break;
			defalut:
				printf("error cmd\n");
				break;
		}
	}
	printf("client sign out\n");
	close(acceptfd);
	return 0;
}

void do_register(int acceptfd, MSG *msg, sqlite3 *db)
{
	char sql[128];
	char *errmsg;
	sprintf(sql, "insert into usr values('%s', '%s');", msg->name, msg->data);
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
		strcpy(msg->data, "usr name already exist");

	}
	else
	{
		printf("client register OK\n");
		strcpy(msg->data, "OK");
	}
	if(send(acceptfd, msg, sizeof(MSG) ,0) < 0)
	{
		perror("send");
		exit(-1);
	}
	return ;
}

void do_login(int acceptfd, MSG *msg, sqlite3 *db)
{
	char sql[128] = {};
	char *errmsg;
	char **result;
	int nrow, ncloumn;

	sprintf(sql, "select * from usr where name='%s' and pass = '%s';",msg->name, msg->data);
	printf("%s\n", sql);
	if(sqlite3_get_table(db, sql, &result, &nrow, &ncloumn, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("get table success\n");
	}

	if(nrow == 1) // 查询成功
	{
		strcpy(msg->data, "OK");	
		send(acceptfd, msg, sizeof(MSG), 0);
	}
	else // 密码或用户名错误
	{
		strcpy(msg->data, "usr/passwd wrong");
		send(acceptfd, msg, sizeof(MSG), 0);
		return ;
	}

}
int do_searchword(int acceptfd, MSG *msg, char word[])
{
	FILE *fp;
	char buf[512];
	int result;
	char *p;
	if((fp=fopen("./dict.txt", "r")) == NULL)
	{
		perror("fopen");
		strcpy(msg->data,"failed to open dict.txt");
		send(acceptfd, msg, sizeof(MSG), 0);
		exit(-1);
	}
	printf("%s, len = %d\n", word, strlen(word));
	while(fgets(buf, 512, fp) != NULL)	
	{
		if((result=strncmp(buf, word, strlen(word))) > 0)
		{
			break;	
		}
		else if(result < 0 || ((result == 0) && *(buf+strlen(word)) != ' '))
		{
			continue;
		}
		p = buf+strlen(word); 
		while(*p == ' ') // 找到注释部分
			p++;
		strcpy(msg->data, p);
		fclose(fp);
		return 1;
	}
	fclose(fp);
	return 0;
}
int get_date(char *date)
{
	time_t t;
	time(&t);
	strcpy(date, ctime(&t));
	return 0;
}

void do_query(int acceptfd, MSG *msg, sqlite3 *db)
{
	char word[64];
	int found = 0;
	char date[128];
	char sql[128];
	char *errmsg;
	strcpy(word, msg->data);

	if((found = do_searchword(acceptfd, msg, word)) == 1)
	{
	// 获取系统时间
		get_date(date);
		printf("%s\n", date);
		sprintf(sql, "insert into record values('%s' ,'%s', '%s');", msg->name, date, word);
		printf("%s\n",sql);
		if(sqlite3_exec(db , sql, NULL, NULL, &errmsg) != SQLITE_OK )
		{
			printf("%s\n", errmsg);
			exit(-1);
		}
	}
	else
	{
		strcpy(msg->data, "Not found");		
	}
	send(acceptfd,  msg,  sizeof(MSG), 0);

}
void do_history(int acceptfd, MSG *msg, sqlite3 *db)
{
	char sql[128];
	int nrow, ncloumn;
	char **result;
	char *errmsg;
	char *p;
	int index, n;
	int i,j;
	sprintf(sql, "select * from record where name='%s';", msg->name);
	printf("%s\n", sql);
	if(sqlite3_get_table(db, sql, &result, &nrow, &ncloumn, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
		exit(-1);
	}
	if(nrow > 0)
	{
		printf("row:%d cloumn:%d\n", nrow, ncloumn);
		index = ncloumn;
		p = msg->data;
		for(i=0; i<nrow; i++)
		{
			for(j=0; j<ncloumn; j++)
			{
				n = index;
				sprintf(p, "%s", result[index++]);
				p = p + strlen(result[n]);
			}
			*p = '\n';
			p++;
		}
		*p = '\0';
		printf("%s\n",msg->data);
		if(send(acceptfd, msg, sizeof(MSG), 0) < 0)
		{
			printf("send");
			exit(-1);
		}
		sqlite3_free_table(result);
	}
	else
	{

		printf("have no history\n");
	}
}
