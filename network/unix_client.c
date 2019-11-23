#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <sys/un.h>

#define UNIX_PATH "/tmp/my_domain_file.1"

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_un sun;
	if((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0))  < 0)
	{
		perror("socket");
		exit(-1);
	}
	bzero(&sun, sizeof(sun));
	sun.sun_family = AF_LOCAL;	
	if(access(UNIX_PATH, F_OK|W_OK) < 0)
	{
		exit(-1);
	}
	strncpy(sun.sun_path, UNIX_PATH, strlen(UNIX_PATH));
	if(connect(sockfd, (struct sockaddr *) &sun, sizeof(sun))  < 0)
	{
		perror("connect");
		exit(-1);
	}
	char buf[128];
	bzero(buf, 128);
	while(1)
	{
		fgets(buf, 128 - 1, stdin);
		if(strncmp(buf, "quit", 4) == 0)
			break;
		if(write(sockfd, buf, strlen(buf)) < 0)
			break;

	}
	close(sockfd);	
	return 0; 
}
