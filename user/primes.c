#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

void 
primes(int file)
{
	int p = 0;
	read(file, &p, 4);
	printf("prime %d\n", p);
	int created = 0;
	int num = 0;
	int fd[2];
	pipe(fd);
	
	while(read(file, &num, 4) != 0)
	{
		if(created == 0)
		{
		//	pipe(fd);
			created = 1;
			int pid = fork();
			if(pid == 0)
			{
				close(fd[1]);
				primes(fd[0]);
				return;
			}
			else
			{
				close(fd[0]);
			}
		}
		if(num % p != 0)
		{
			write(fd[1], &num, 4);
		}
	}
	close(file);
	close(fd[1]);
	wait(0);
}
int
main(int argc, char *argv[])
{
	int fd[2];
	pipe(fd);
	int pid = fork();
	if(pid != 0)
	{
		close(fd[0]);
		for(int i = 2; i <= 35; i++)
		{
			write(fd[1], &i, 4);
		}
		close(fd[1]);
		wait(0);
	}
	else
	{
		close(fd[1]);
		primes(fd[0]);
		close(fd[0]);
	}
	exit(0);
}
