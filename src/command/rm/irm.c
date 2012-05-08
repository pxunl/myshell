/*
 ************************************************************************************
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ************************************************************************************
 * Filename     :  irm.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Monday, May 07, 2012 01:31:10 HKT
 * Description  :  rm removes each pecified file.  By default, it does not remove directories.
 * 				   with -d Remove the DIRECTORY(ies), if they are empty.
 * 				   with -D Remove the DIRECTORY(ies), no matter wheater they are empty or not.
 * History      :
 * Revision     :  none
 * Compiler     :  gcc
 *
 *************************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>

#define R_TRUE   0
#define R_FALSE  -1


/**
 * @do_rm_files: delete files
 * @file
 * @return: R_FALE or R_TRUE
 */
int do_rm_files(char *file)
{
	if (!(*file)) 
	{
		return R_FALSE;
	}
	return remove(file);
}


/**
 * @do_rm_empty_dirs: delete dirs if they are empty
 * @dir;directory name
 * @return: R_TRUE or R_FALSE 
 */
int do_rm_empty_dirs(char *dir)
{
	if (!(*dir)) 
	{
		return R_FALSE;
	}

	return rmdir(dir);
}


/**
 * @do_rm_no_empty_dirs: delete directorys through "system(rm -rf)" if they are not empty
 * @dir: directory name
 * @return: R_TRUE or R_FALSE
 */
int do_rm_no_empty_dirs(char *dir)
{
	if (!(*dir)) 
	{
		return R_FALSE;
	}
	char *cmd = (char *)malloc(sizeof(char) * (8 + strlen(dir)));
	strcpy(cmd, "rm -rf ");
	strcat(cmd, dir);
	return system(cmd);
}

int main(int argc, char *argv[])
{
	if (argc < 2) 
	{
		perror("Usage: missing directory or file names\n");
		exit(1);
	}

	int i;
	int rm_empty_flag = 0;
	int rm_no_empty_flag = 0;
	for (i = 1; i < argc; i++) 
	{
		if (strstr(argv[i], "-d") != NULL) 
		{
			rm_empty_flag = 1;
		}
		else if (strstr(argv[i], "-D") != NULL)
		{
			rm_no_empty_flag = 1;
		}
	}

	for (i = 1; i < argc; i++) 
	{
		if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "-D") == 0) 
		{
			continue;
		}
		if (1 == rm_no_empty_flag) 
		{
			if (do_rm_no_empty_dirs(argv[i]) == R_FALSE)
			{
				printf("\nUnable to delete directory %s\n", argv[i]);
			}
		}
		else if (0 == rm_no_empty_flag && 1 == rm_empty_flag) 
		{
			if (do_rm_empty_dirs(argv[i]) == R_FALSE)
			{
				printf("\nUnable to delete directory %s\n", argv[i]);
			}
		}
		else if (0 == rm_empty_flag && 0 == rm_no_empty_flag) 
		{
			if (do_rm_files(argv[i]) != R_TRUE) 
			{
				printf("unable to delete file %s\n", argv[i]);
			}
		}
		
	}
	printf("Finish...\n");
	return 0;
}
