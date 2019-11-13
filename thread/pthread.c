#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
char message[32] = "Hello World";
void* thread_func(void *arg);

int main()
{
	pthread_t a_thread;
	void *result;
	if(pthread_create(&a_thread, NULL, thread_func, NULL) != 0)
	{	
		puts("fail to pthread_create");
		exit(-1);
	}
	pthread_join(a_thread, &result);
	
	printf("result is %s\n", (char *)result);
	printf("message is %s\n", message);

	return 0;
}

void* thread_func(void *arg)
{
	sleep(1);
	strcpy(message, "marked by thread");
	pthread_exit("thank you for waiting for me");
}
