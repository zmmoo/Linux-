#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#define _LOCK_ // == gcc -D_LOCK_

void *func(void *arg);
pthread_mutex_t mutex;

int count = 0;
int value1, value2;
int main()
{
	pthread_t pthread;
	if(pthread_mutex_init(&mutex, NULL)<0)
	{
		perror("pthread_mutex_init");
		exit(-1);
	}
	if(pthread_create(&pthread,NULL,func,NULL)!= 0)
	{
		perror("pthread_init");
		exit(-1);
	}
	while(1)
	{
		count++;
#ifdef    _LOCK_
		pthread_mutex_lock(&mutex);
#endif
		value1 = count;
		value2 = count;		
#ifdef    _LOCK_
		pthread_mutex_unlock(&mutex);
#endif
	}
	return 0;
}
void *func(void *arg)
{
	while(1)	
	{
#ifdef    _LOCK_
		pthread_mutex_lock(&mutex);
#endif
		
		if(value1 != value2)
		{
			printf("value1:%d, value2:%d\n", value1, value2);
			sleep(2);
		}
#ifdef    _LOCK_
		pthread_mutex_unlock(&mutex);
#endif
	}
}
