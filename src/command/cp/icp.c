/*
 ************************************************************************************
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ************************************************************************************
 * Filename     :  icp.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Sunday, May 06, 2012 09:44:56 HKT
 * Description  :  Copy SOURCE to DEST, or multiple SOURCE(s) to DIRECTORY.
  				   -d copy directories recursively
  				   As a special case, cp makes a backup of SOURCE when the  force  and
       		       backup  options are given and SOURCE and DEST are the same name for
       			   an existing, regular file.
 * History      :
 * Revision     :  Bug: can't copy direcory(Segmentation fault)
 * 				        can't open and copy some files,expecially in gtk_windows
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

#define  BUF_SIZE   256

typedef enum 
{
	FAIL,
	SUCCEED
}Exit_Code;

void copy_file(char *src, char *dest);
int do_copy(char *src, char *dest);

int main(int argc, char *argv[])
{
	if (argc < 3) 
	{
		perror("Usage: icp source dest\n");
		exit(1);
	}
	
	int i;
	for (i = 1; i < argc - 1; i++) 
	{
		do_copy(argv[i], argv[argc-1]);
	}

	printf("Finish...\n");
	
	return 0;
}


/**
 * @do_copy: copy from src to dest,
 * 			 recursive calling if it is a directory
 * @src: source file/directory
 * @dest: destination
 * @return: FAIL or SUCCEED
 **/
int do_copy(char *src, char *dest)
{
	if (!(*src) || !(*dest)) 
	{
		return FAIL;
	}
	
	DIR *src_dir;

	char *src_str= (char *)malloc(sizeof(char) * strlen(src));
	char *dest_str = (char *)malloc(sizeof(char) * strlen(dest));
	strcpy(src_str, src);
	strcpy(dest_str, dest);

	struct dirent *entry;
	struct stat   stat_buf;

	/* it will be support later "-d" */
	if (strcmp(src, "-d") == 0) 
	{
		return 0;
	}

	/* if src is not a directory (is a normal file) */
	if ((src_dir= opendir(src)) == NULL)
	{
		/*fprintf(stderr,"ils: cannot open %s\n", dirname);*/
		copy_file(src, dest);	
	}
	else
	{
		while ((entry = readdir(src_dir)) != NULL)
		{
			lstat(entry->d_name, &stat_buf);

			/* we should create  new directorys if src contains directorys */
			if (S_ISDIR(stat_buf.st_mode)) 
			{
				if ((strcmp(".", entry->d_name) == 0)
						|| (strcmp("..", entry->d_name) == 0))
				{
					continue; /* ignore '.' and '..' */
				}

				/* new direcory name */
				strcat(dest_str, "/");
				strcat(dest_str, entry->d_name);

				mkdir(dest_str, S_IRWXU);
				
				/*chdir(src);*/
				strcat(src_str, "/");
				strcat(src_str, entry->d_name);

				do_copy(src_str, dest_str);

				/* back to previous directory */
				strcpy(src_str, src);
				strcpy(dest_str, dest);
				/*chdir("..");*/
			}
			else  /* copy common files */
			{
				copy_file(src, dest);	
				return SUCCEED;
				/*printf("%s\n", direntp->d_name);*/
			}
		}
		/*closedir(dir_ptr);*/
	}
	return FAIL;
}

void copy_file(char *src, char *dest)
{
	FILE *src_fp  = fopen(src, "r");

	char *new_file = NULL;
	
	/* get new file name */
	if ((new_file = strrchr(src, '/')) == NULL)
	{
		strcat(dest, "/");
		strcat(dest, src);
	}
	else 
	{
		strcat(dest, new_file);
	}
	
	FILE *dest_fp = fopen(dest, "w+");
	char *buf[BUF_SIZE];
	int  rev_size = 0;

	if (src_fp == NULL || dest_fp == NULL) 
	{
		printf("can't open file %s", src);
		printf("or %s", dest);
		return;
	}

	while ((rev_size = fread(buf, BUF_SIZE, 1, src_fp)) > 0)
	{
		fwrite(buf, BUF_SIZE, rev_size, dest_fp);
	}
}
