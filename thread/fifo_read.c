#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd;
	char buf[32];
	if((fd = open("./myfifo", O_RDONLY))< 0 )
	{
		perror("open");
		exit(-1);
	}
	printf("open my fifo\n");
	while(read(fd, buf, 32) > 0)
		printf("read %ld\n", strlen(buf));
	close(fd);
	return 0;
}

