#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


int execute(char *argv[])
{
	int pid_f;
	int chlid_in = -1;

	if (argv[0] == NULL)
		return 0;

	if ((pid_f = fork()) == -1)
	{
		perror("fork fail\n");
	}
	else if (pid_f == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execlp(argv[0], argv);
		perror("not found!\n");
		exit(1);
	}
	else
	{	
		if (wait(&chlid_in) == -1)
			perror("wait\n");
	}
	return chlid_in;
}
