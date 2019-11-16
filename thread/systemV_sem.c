#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define N 64
#define READ 0
#define WRITE 1

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short*array;
	struct seminfo *__buf;
};
void init_sem(int semid, int s[], int n)
{
	int i;
	union semun myun;
	for(i=0; i<n; i++)
	{
		myun.val = s[i];
		semctl(semid, i, SETVAL, myun);
	}
}

void pv(int semid, int num, int op)
{
	struct sembuf buf;
	buf.sem_num = num;
	buf.sem_op = op;
	buf.sem_flg = 0;
	semop(semid, &buf, 1);
}

int main()
{
	int shmid, semid ,s[] = {0,1};
	pid_t pid;
	key_t key;
	char *shmaddr;
	if((key=ftok(".", 's')) < 0)
	{
		perror("ftok");
		exit(-1);
	}
	if((shmid=shmget(key, N, IPC_CREAT|0666)) < 0)
	{
		perror("shmget");
		exit(-1);
	}
	if((semid = semget(key, 2, IPC_CREAT|0666)) < 0)
	{
		perror("semget");
		goto _error1;
	}
	init_sem(semid, s, 2);
	if((shmaddr= (void *)shmat(shmid, NULL, 0)) == (void *) -1)
	{
		perror("shmat");
		goto _error2;
	}
	if((pid=fork()) < 0)
	{
		perror("fork");
		goto _error2;
	}
	else if(pid == 0)
	{
		char *p, *q;
		while(1)	
		{
			pv(semid,READ, -1);
			p = q = shmaddr;
			while(*q)
			{
				if(*q != ' ')
					*p++ = *q;
				q++;
			}
			*p = '\0';
		printf("%s\n", shmaddr);
		pv(semid, WRITE, 1);
		}
	}
	else 
	{
		while(1)	
		{
			pv(semid, WRITE, -1);
			printf("input > ");
			fgets(shmaddr, N, stdin);
			if(strcmp(shmaddr, "quit\n") == 0)
				break;
			pv(semid, READ, 1);
		}
		kill(pid, SIGUSR1);
	}
_error2:
	semctl(semid, 0, IPC_RMID);
_error1:
	shmctl(shmid, IPC_RMID, NULL);
	 return 0;	
}
