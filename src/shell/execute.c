#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>


int execute(char *argv[])
{
	int pid_f;
	int chlid_in = -1;

	if (argv[0] == NULL)
		return 0;
	
	/*fork a new procee to execute the command*/
	if ((pid_f = fork()) == -1)
	{
		perror("fork fail\n");
	}

	/*child*/
	else if (pid_f == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execvp(argv[0], argv);
		perror("not found!\n");
		exit(1);
	}

	/*father*/
	else
	{	
		if (wait(&chlid_in) == -1)
			perror("wait\n");
	}

	return chlid_in;
}
