#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
	int fd1[2];
	int fd2[2];
	if(pipe(fd1) < 0)
	{
		printf("create pipe failure\n");
		exit(1);
	}
	if(pipe(fd2) < 0)
	{
		printf("create pipe failure\n");
		exit(1);
	}
	int pid;
	pid  = fork();
	if(pid == 0)
	{
		char str2[10];
		read(fd2[0], str2, sizeof(str2));
		printf("%d: received pong\n", getpid());
		write(fd1[1], "p", 1);
	}
	else
	{
		char str1[10] = "p";
		write(fd2[1], "p", 1);
		read(fd1[0], str1, sizeof(str1));
		printf("%d: received ping\n", getpid());
	}
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
	exit(0);
}
