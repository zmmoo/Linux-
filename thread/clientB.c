#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

typedef struct
{
	long mtype;
	char mtext[64];
}MSG;

#define LEN (sizeof(MSG)- sizeof(long))
#define TypeA 100
#define TypeB 200

int main()
{
	MSG buf;
	key_t key;
	int msgid;
	if((key = ftok(".", 'q')) < 0)
	{
		perror("ftok");
		exit(-1);
	}
	if((msgid=msgget(key, IPC_CREAT|0666)) < 0)
	{
		perror("msgget");
		exit(-1);
	}
	while(1)
	{
		if(msgrcv(msgid, &buf, LEN, TypeB, 0) < 0)
		{
			perror("msgrcv");
			exit(-1);
		}
		if(strcmp(buf.mtext, "quit\n") == 0)
		{
			msgctl(msgid, IPC_RMID, NULL);
			exit(0);
		}
		printf("recv form clienA:%s", buf.mtext);
		buf.mtype = TypeA;
		printf("input >\n");
		fgets(buf.mtext, 64, stdin);
		msgsnd(msgid, &buf, LEN, 0);
		if(strcmp(buf.mtext, "quit\n") == 0)
			break;
	}
	return 0;
}
