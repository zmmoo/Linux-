#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

void* func(void *argv);
char buf[32];
sem_t sem_r, sem_w;

int main()
{
	pthread_t pthread;
	if(sem_init(&sem_r, 0, 0)<0)
	{
		perror("sem_init()");
		exit(-1);
	}
	if(sem_init(&sem_w, 0, 1)<0)
	{
		perror("sem_init()");
		exit(-1);
	}
	if(pthread_create(&pthread, NULL, func, NULL) != 0)
	{
		perror("pthread_create");
		exit(-1);
	}
	puts("input 'quit' to exiti\n");
	do{
		sem_wait(&sem_w);
		//fgets(buf, 32, stdin);
		scanf("%s", buf);
		sem_post(&sem_r);
	}while(strncmp(buf, "quit", 4) != 0);
	return 0;
}
void *func(void *argv)
{
	while(1)
	{
		sem_wait(&sem_r);
		printf("string length is %ld\n", strlen(buf));
		sem_post(&sem_w);
	}
}
