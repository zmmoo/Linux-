#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int signo)
{
	if(signo == SIGINT)
		printf("i have got SIGINT\n");
	if(signo == SIGQUIT)
		printf("i have got SIGQUIT\n");
}
int main()
{
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	while(1)
		pause();
	return 0;
}
