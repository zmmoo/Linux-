#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
	pid_t pid1,pid2;
	char buf[32];
	int pfd[2];
	if(pipe(pfd) < 0)
	{
		perror("pipe");
		exit(-1);
	}
	if((pid1=fork())<0)
	{
		perror("fork");
		exit(-1);
	}
	else if(pid1 == 0)
	{
		close(pfd[0]);
		strcpy(buf, "i'm process1");
		write(pfd[1], buf, 32);
		exit(0);
	}
	else 
	{
		if((pid2=fork()) < 0)
		{
			perror("fork");
			exit(-1);
		}
		else if(pid2 == 0)
		{
			sleep(1);
			close(pfd[0]);
			strcpy(buf, "i'm process 2");
			write(pfd[1], buf, 32);
		}
		else
		{
			close(pfd[1]);
			wait(NULL);
			read(pfd[0], buf, 32);
			printf("%s\n", buf);
			wait(NULL);
			read(pfd[0], buf, 32);
			printf("%s\n", buf);

		}
	}
	return 0;
}
