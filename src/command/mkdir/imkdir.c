/*
 ************************************************************************************
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ************************************************************************************
 * Filename     :  imkdir.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Sunday, May 06, 2012 09:44:56 HKT
 * Description  :  Create the DIRECTORY(ies), if they do not already exist.
 * 				   Mandatory arguments to long options are mandatory for short options 
 * 				   too.
 * History      :
 * Revision     :  
 * Compiler     :  gcc
 *
 *************************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>


typedef enum 
{
	R_TRUE,
	R_FALSE
}Exit_Code;

int check_name(char *name);
int do_mkdir(char *dir);

int main(int argc, char *argv[])
{
	if (argc < 2) 
	{
		perror("Usage: missing new directory name\n");
		exit(1);
	}
	
	if (check_name(argv[1]) == R_FALSE)
	{
		perror("valdlity directory name\n");
		return 0;
	}
	if (R_FALSE == do_mkdir(argv[1])) 
	{
		printf("Fail\n");
		return 0;
	}
	printf("Finish...\n");
	return 0;
}


/**
 * @do_mkdir: make a new directory
 * @dir: directory name
 * @return: FAIL or SUCCEED
 **/
int do_mkdir(char *dir)
{
	if (!(*dir)) 
	{
		return R_FALSE;
	}
	mkdir(dir, S_IRWXU);
	return R_TRUE;
}

/*
 * @Check_Name 
 * description: check name
 *            : valid----[0-9] in head, or contains ' ' ,'=',','  or contains a none-char
 *
 * @name: input string
 * @return : R_TRUE or R_FALSE
 */
int check_name(char *name)
{
	if (!name) 
	{
		return R_FALSE;
	}

	char * ck = name;

	if (isdigit(*ck))
	{
		return R_FALSE;
	}

	while (*ck)
	{
		if (*ck == ' ' || *ck == '=' || !(isalnum(*ck))) 
		{
			return R_FALSE;
		}
		ck++;	
	}
	return R_TRUE;
}


