#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
int main()
{
	pid_t id;
	int status;
	if((id = fork()) < 0)
	{
		perror("fork ");
		return -1;
	}
	if(id == 0)
	{
		printf("this is child %d\n", getpid());
		execl("/bin/ls", "ls", "-al", "./", NULL);
		printf("+++++++++++++++++++++++++++++\n");
	}
	else
	{
		wait(&status);
		printf("this is father %d, my child is %d\n", getpid(), id);
		execl("/bin/ls", "ls", "-al", "./", NULL);
		printf("+++++++++++++++++++++++++++++\n");
		printf("%x\n", status);
	}
	return 0;
}
