#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	alarm(3);
	pause();
	printf("hello\n");
	return 0;
}
