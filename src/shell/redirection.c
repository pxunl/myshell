/*
 ************************************************************************************
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ************************************************************************************
 * Filename     :  redirection.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Tuesday, May 22, 2012 08:36:56 HKT
 * Description  :  process redirections;
 * 				   command lines that contain '>' will be look as input redirection.
 * 				   command lines that contain '<' will be look as output redirection.
 * 				   command lines that contain ">>" will be redirection to the end of 
 * 				   the file(discritor).
 *
 * Note         :  Different from Bash Shell, it support parallel redirections.
 * 				   And just one file is allowed after one redirection descriptor,
 * 				   otherwise, mshell will pop up illegal information.
 * History      :
 * Revision     :  none
 * Compiler     :  gcc
 *
 *************************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <gtk/gtk.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "general.h"
#include "config.h"
#include "execute.h"
#include "shell.h"
#include "redirection.h"

#define  EXPECT_FILE   			 1
#define  EXPECT_SINGAL_SYMMBOL   2
#define  EXPECT_DOUBLE_SYMBOL    3
#define  EXPECT_CMD 			 4
#define  EXPECT_ANYTHING         5
#define  REDIRECTION_NUM         4

#define  STD_IN 				 0
#define  STD_OUT				 1
#define  STD_OUT_ERR 			 2
#define  oops(msg, x)   {perror(msg); exit(x);}
/*extern gint apipe[2];  [> from mainWindow.c <]*/

int (*redirect_table[REDIRECTION_NUM])(char **program, struct REDIRECT *redirection) = {do_redirect_output,
																					do_redirect_input,
																					do_redirect_double_output,
																					do_redirect_pipe
																					};
char *redirection_descriptor[REDIRECTION_NUM] = {">", "<", ">>", "|"};


char **get_program(char **cmd)
{
	char **program = NULL;
	int program_len = 0;
	while ((cmd[program_len] != NULL) && (is_in_descriptor_table(cmd[program_len]) == R_FALSE))
	{
		program_len++;
	}
	
	/* copy orignal command line from cmd to program_parameters */
	/* before we use program_parameters as a character array,
	 * we should allocate some memory so that we can use indexs
	 * ,that will make it look like character array.
	 * so, we have to know how many index numbers we need before allocating.
	 */
	program = (char **)malloc((program_len+1) * sizeof(char *));
	int i = 0;
	/* get program name and parameters */
	while ((cmd[i] != NULL) && (is_in_descriptor_table(cmd[i]) == R_FALSE))
	{
		program[i] = (char *)malloc((strlen(cmd[i]) + 1) * sizeof(char));
		if (program[i] == NULL) 
		{
			g_print("Out of memory!\n");
			exit(1);
		}
		strcpy(program[i], cmd[i]);
		i++;
	}
	program[i] = NULL;
	return program;
}


/**
 * @get_redirection_list
 * @description: get redirection link for '>' '<' '>>'.
 * 				 If there is '|' in it, we just create one REDIRECT node to
 * 				 record something necessary information about pipe.
 * 				 This node will contains a program name which is on 
 * 				 the right of the '|' in re_file.
 *
 * @cmd: orignal command lines that have redirection descriptors.
 * @return: head that point to the link of redirection link.
 */
struct REDIRECT *get_redirection_list(char ** cmd)
{
	char **program_parameters = NULL;
	int i = 0;
	int count = 0;

	/* how many parameters are in cmd */
	while (cmd[count] != NULL)
	{
		count++;
	}
	
	/* copy orignal command line from cmd to program_parameters */
	/* before we use program_parameters as a character array,
	 * we should allocate some memory so that we can use index
	 * ,that will make it look like character array.
	 * so, we have to know how many index numbers we need before allocating.
	 */
	program_parameters = (char **)malloc((count + 1) * sizeof(char *));
	/* get one copy of original command lines */
	while (cmd[i] != NULL)
	{
		program_parameters[i] = (char *)malloc((strlen(cmd[i]) + 1) * sizeof(char));
		if (program_parameters[i] == NULL) 
		{
			g_print("Out of memory!\n");
			exit(1);
		}
		strcpy(program_parameters[i], cmd[i]);
		i++;
	}
	program_parameters[i] = NULL;
	

	int j = 0;
	struct REDIRECT *phead = (struct REDIRECT *)malloc(sizeof(struct REDIRECT));
	struct REDIRECT *ptail= (struct REDIRECT *)malloc(sizeof(struct REDIRECT));
	phead->next = NULL;
	ptail->next = NULL;
	ptail = phead;
	struct REDIRECT *add_node = NULL;

	/* get link and set re_file word */
	while (program_parameters[j] != NULL)
	{
		/*if ((strcmp(program_parameters[j], ">") == 0)*/
		if ((is_in_descriptor_table(program_parameters[j]) == R_TRUE)
				&& (program_parameters[j+1] != NULL)
				&& (Check_File_Name(program_parameters[j+1]) == R_TRUE))
		{
			add_node = (struct REDIRECT *)malloc(sizeof(struct REDIRECT));
		
			add_node->word = (char *)malloc(sizeof(char) * (strlen(program_parameters[j]) + 1));
			strcpy(add_node->word, program_parameters[j]);
			
			add_node->re_file = (char *)malloc((strlen(program_parameters[j+1]) + 1) * sizeof(char));
			strcpy(add_node->re_file, program_parameters[j+1]);
			add_node->next = NULL;
			ptail->next = add_node;
			ptail = add_node;
		}
		j++;
	}

	struct REDIRECT *travel = phead;
	travel = travel->next;
	/* set red_type */
	/////////////////////////////////////////////////
	////how aboubt 'case'
	while (travel != NULL)
	{
		if (strcmp(travel->word, redirection_descriptor[0]) == 0) 
		{
			travel->red_type = Re_singal_out;
		}
		else if (strcmp(travel->word, redirection_descriptor[1]) == 0) 
		{
			travel->red_type = Re_singal_in;
		}
		else if (strcmp(travel->word, redirection_descriptor[2]) == 0) 
		{
			travel->red_type = Re_double_out;
		}
		else if (strcmp(travel->word, redirection_descriptor[3]) == 0) 
		{
			travel->red_type = Re_pipe;
		}
		
		travel = travel->next;
	}

	free_temp_cmd(program_parameters);
	return phead;
}

/**
 * @free_temp_cmd
 * @description: free memory of temp command lines.
 *
 * @cmd: temp command liens.
 */
void free_temp_cmd(char **cmd)
{
	char **clear = cmd;
	while (*clear)
	{
		free(*clear++);
	}
	free(cmd);
	cmd = NULL;
}


/**
 * @Need_Redirection
 * @descripton: if command lines contain '<', '>', '>>', '<<',
 * 			    we should redirect it.(to the file discriptor next to it)
 *
 * @cmd: command line.
 * @return: R_TRUE, R_FALSE
 */
int Need_Redirection(char **cmd)
{
	int i = 0;
	while (cmd[i] != NULL)
	{
		if (is_in_descriptor_table(cmd[i]) == R_TRUE)
		{
			return R_TRUE;
		}
		i++;
	}
	return R_FALSE;
}


/**
 * @is_in_descriptor_table
 * @description: check if it is in descriptor table
 * @str: specical descriptor
 *
 * @return R_TRUE, R_FALSE
 */
int is_in_descriptor_table(char * str)
{
	int i = 0;
	for (i = 0; i < REDIRECTION_NUM; i++) 
	{
		if (strcmp(redirection_descriptor[i], str) == 0) 
		{
			return R_TRUE;
		}
	}
	return R_FALSE;
}


/**
 * @Check_Redirection_Validity 
 * @description: check if the command lines that contain 'redirection chars'
 * 	   		     is legal or not.
 * 			     Legal: cmd > file, cmd>file, cmd > file > another,
 * 			     program_left | program_right.
 * 			     Illegal: cmd > file_1 file_2, cmd > > file,
 * 			     program_left |program_right ... and so on.
 * @Note: Any situation that is not listed above will be just given to system(). 
 * @cmd: command lines
 *
 * @return: R_TRUE or R_FALSE.
 */
int Check_Redirection_Validity(char **cmd)
{
	int flag = R_TRUE;
	if ((cmd[0] == NULL)
			|| (strchr(cmd[0], '>') != NULL) 
			|| (strchr(cmd[0], '|') != NULL)
			|| (strchr(cmd[0], '<') != NULL)
			|| (strstr(cmd[0], ">>") != NULL))
	{
		Usage_Redirection(EXPECT_CMD);
		return R_FALSE;
	}
	
	int i = 1;
	while (cmd[i] != NULL)
	{
		if (is_in_descriptor_table(cmd[i]) == R_TRUE)
		{
			if ((cmd[i+1] != NULL) 
					&& (is_in_descriptor_table(cmd[i+1]) == R_FALSE) 
				    && (Check_File_Name(cmd[i+1]) == R_TRUE))
			{
				i += 2;
				continue;
			}
			/* Illegal: program > >
			 * 		    program | |
			 * 		    ... */
			else  
			{
				return R_FALSE;
			}
			
		}
		else 
		{
			if (Check_File_Name(cmd[i]) == R_FALSE)
			{
				return R_FALSE;
			}
		}
		i++;
	}
	
	return flag;
}

/*
 * @Check_File_Name 
 * @description: check name
 *              valid----[0-9] in head, or contains ' ' ,'=',','  
 *              or contains a none-char
 *
 * @name: input string
 * @return : R_TRUE or R_FALSE
 */
int Check_File_Name(char *file)
{
	if (!file) 
	{
		return R_FALSE;
	}

	char * ck = file;

	if (isdigit(*ck))
	{
		return R_FALSE;
	}

	while (*ck)
	{
		/*if (*ck == ' '*/
				/*|| *ck == '=' */
				/*|| !(isalnum(*ck)) */
				/*|| *ck == '>' */
				/*|| *ck == '<'*/
				/*|| ) */
		if ((isalnum(*ck) == 0) && *ck != '.')
		{
			return R_FALSE;
		}
		ck++;	
	}
	return R_TRUE;
}


/**
 * @Usage_Redirection 
 * @description: Usage for redirections command lines.
 */
void Usage_Redirection(int status)
{
	if (status == EXPECT_FILE) 
	{
		g_print("\nexpect file after input or output direction symbol\n");
	}
	else if (status == EXPECT_SINGAL_SYMMBOL) 
	{
		g_print("\nexpect input or output direction symbol before file\n");
	}
	else if (status == EXPECT_CMD) 
	{
		g_print("\n%s\n%s"
				,"ilegal command program."
				, "Try directions --help for more information");
	}
	else 
	{
		g_print("Try directions --help for more information.\n");
		return;
	}
	return;
}


/**
 * @Process_Redirection: 
 * @description: redirecte by pipeline.
 * 				 '>>' : will be redirected and added ouput to 
 * 				 the end of the file(discriptor).
 * 				 (only one file is allowed after one redirecton descriptor.
 * 				 Otherwise, system call "system()" will pop up wrong messages.)
 * @cmd: command lines
 * @return: R_TRUE or R_FALSE.
 */
int Process_Redirection(char **cmd)
{
	struct REDIRECT *ph= get_redirection_list(cmd);
	if (ph == NULL || cmd == NULL) 
	{
		return R_FALSE;
	}
	
	int flag = R_FALSE;
	char **program = get_program(cmd);
	ph = ph->next;
	while (ph!= NULL)
	{
		/* findout what kind of redirection it want. */
		switch(ph->red_type)
		{
			case Re_singal_out:
				flag = redirect_table[0](program, ph);
				break;
			case Re_singal_in:
				flag = redirect_table[1](program, ph);
				break;
			case Re_double_out:
				flag = redirect_table[2](program, ph);
				break;
			case Re_pipe:
				flag = redirect_table[3](program, ph);
				/*dup2(apipe[1], 1);*/
				/*dup2(apipe[1], 2);*/
				break;
			default: Redirection_Usage();
					 break;
		}
		ph = ph->next;
	}
	free_temp_cmd(program);
	free_direction_list(ph);
	return flag;
}

void free_direction_list(struct REDIRECT *pfree)
{
	struct REDIRECT *ptmp = NULL;
	while (NULL != pfree)
	{
		ptmp = pfree;
		pfree = pfree->next;
		free(ptmp);
		ptmp = NULL;
	}
	pfree = NULL;
}


/**
 * @do_redirect_output
 * @description: fork a child process, and then redirect files 
 * 				 descriptors for output redirections.
 *
 * @program: program name and it's parameters(just one program once is allowed).
 * @redirection: links that contain redirections information.
 * @return: R_FALSE, R_TRUE
 */
int do_redirect_output(char **program, struct REDIRECT *redirection) 
{
	if (program == NULL || redirection == NULL) 
	{
		return R_FALSE;
	}
	
	int pid_f;
	int chlid_in = R_FALSE;
	/*extern char **environ;*/
	
	/*fork a new procee to execute the command*/
	if ((pid_f = fork()) == -1)
	{
		perror("fork fail");
		return R_FALSE;
	}

	/*child process*/
	else if (pid_f == 0)
	{
		/*environ = Table_To_Env();*/
		int redir_file;
		if ((redir_file = open(redirection->re_file, O_RDWR | O_CREAT, S_IRUSR)) == -1)
		{
			exit(1);
		}
		close(STD_OUT);
		dup(redir_file);
		close(redir_file);
		execvp(program[0], program);
		perror("illegal command!\n");
		exit(1);	
	}
	else
	{	
		if (wait(&chlid_in) == -1)
		{
			chlid_in = R_FALSE;
			perror("wait\n");
		}
		chlid_in = (WIFEXITED(chlid_in)? R_TRUE: R_FALSE);
	}
	return chlid_in;
}


/**
 * @do_redirect_input
 * @description: fork a child process, and then redirect files 
 * 				 descriptors for input redirections.
 *
 * @program: program name and it's parameters(just one program once is allowed).
 * @redirection: links that contain redirections information.
 * @return: R_FALSE, R_TRUE
 */
int do_redirect_input(char **program, struct REDIRECT *redirection) 
{
	if (program == NULL || redirection == NULL) 
	{
		return R_FALSE;
	}
	
	int pid_f;
	int chlid_in = R_FALSE;
	/*extern char **environ;*/
	
	/*fork a new procee to execute the command*/
	if ((pid_f = fork()) == -1)
	{
		perror("fork fail");
		return R_FALSE;
	}

	/*child process*/
	else if (pid_f == 0)
	{
		/*environ = Table_To_Env();*/
		int redir_file;
		if ((redir_file = open(redirection->re_file, O_RDONLY, S_IWGRP)) == -1)
		{
			exit(1);
		}
		close(STD_IN);
		dup(redir_file);
		close(redir_file);
		execvp(program[0], program);
		perror("illegal command!\n");
		exit(1);	
	}
	else
	{	
		if (wait(&chlid_in) == -1)
		{
			chlid_in = R_FALSE;
			perror("wait\n");
		}
		chlid_in = (WIFEXITED(chlid_in)? R_TRUE: R_FALSE);
	}
	
	return R_FALSE;
}

/**
 * @do_redirect_double_output
 * @description: fork a child process, and then redirect files 
 * 				 descriptors for double_output redirections.
 * 				 This will add output data to the end of the file that
 * 				 has been pointed by file descriptor.
 * @program: program name and it's parameters(just one program once is allowed).
 * @redirection: links that contain redirections information.
 * @return: R_FALSE, R_TRUE
 */
int do_redirect_double_output(char **program, struct REDIRECT *redirection) 
{
	return R_FALSE;
}


/**
 * @Redirection_Usage 
 * @description: give help information or usage.
 */
void Redirection_Usage()
{
	g_print("\n------Finished------\n");
}

int do_redirect_pipe(char **program, struct REDIRECT *redirection)
{
	if (program == NULL || redirection == NULL) 
	{
		return R_FALSE;
	}
	
	int re_pipe[2] = {0, 0};
	int pid_f;
	int r_parent = R_TRUE;
	int r_chind = R_TRUE;
	char **right_program = NULL;
	
	right_program = (char **)malloc(2 * sizeof(char *));
	right_program[0] = (char *)malloc((strlen(redirection->re_file) + 1) * sizeof(char));
	strcpy(right_program[0], redirection->re_file);
	right_program[1] = NULL;
	
	if (pipe(re_pipe) == -1) 
	{
		oops("Can't get a new pipe", 1);
	}
	
	if ((pid_f = fork()) == -1)
	{
		oops("Can't fork!", 2);
		return R_FALSE;
	}

	/*child process exec program[1] and writes
	 * into pipe. But it does't read from pipe */
	else if (pid_f == 0)
	{
		if ((pid_f = fork()) == -1)
		{
			oops("Can't fork!", 2);
			return R_FALSE;
		}

		close(re_pipe[0]);
		if (dup2(re_pipe[1], 1) == -1)
		{
			oops("Cloud't redirect stdout", 3);
		}
		close(re_pipe[1]);
		r_chind = (execvp(program[0], program) == -1)? R_FALSE : R_TRUE;
		oops(program[0], 3);

		if (pid_f == 0)
		{
			close(re_pipe[1]);
			if (dup2(re_pipe[0], 0) == -1)
			{
				oops("Cloud't redirect stdout", 3);
			}
			close(re_pipe[0]);

			if ((redirection->red_type == Re_pipe) && (strcmp(redirection->word, redirection_descriptor[3]) == 0))
			{
				r_parent = (execvp(right_program[0], right_program) == -1)? R_FALSE : R_TRUE;
			}
			oops(right_program[0], 3);
		}
	}

	/* parent prcocess will read from pipe and exec program[0],
	 * but it does't write to pipe. */

	free_temp_cmd(right_program);
	return ((r_chind == R_TRUE) && (r_parent == R_TRUE))? R_TRUE : R_FALSE;
}
