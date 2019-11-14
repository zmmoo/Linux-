#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
	int fd;
	char buf[32];
	if((fd = open("./myfifo", O_WRONLY))< 0 )
	{
		perror("open");
		exit(-1);
	}
	printf("open fifo success\n");
	while(1)
	{
		fgets(buf, 32, stdin);
		if(strncmp(buf,"quit", 4) == 0)
			break;
		write(fd, buf, 32);
		
	}
	close(fd);
	return 0;
}

