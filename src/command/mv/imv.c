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
 * 			 recursive calling if it is a directory.
 * @src: source file/directory
 * @dest: destination, it should not contain '/' at the end of the 'dest' string
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

	stat(src, &stat_buf);

	/* if src is not a directory (is a normal file) */
	if (S_ISREG(stat_buf.st_mode))
	{
		copy_file(src, dest);
		return SUCCEED;
	}
	
	/* if src is a directory */
	else if (S_ISDIR(stat_buf.st_mode))
	{
		strcat(dest_str, "/");
		strcat(dest_str, src);
		mkdir(dest_str, S_IRWXU);
		if ((src_dir = opendir(src)) == NULL)
		{
			fprintf(stderr,"ils: cannot open %s\n", src);
			return FAIL;
		}
		else
		{
			chdir(src);  /* we need to go into src
		   				    only we change the current directory to be 'src',
						    we can use relative path, otherwise we can only use
						    absolute path */
			while ((entry = readdir(src_dir)) != NULL)
			{
				stat(entry->d_name, &stat_buf);

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

					strcat(src_str, "/");
					strcat(src_str, entry->d_name);

					do_copy(src_str, dest_str);

					/* back to previous directory */
					memset(src_str, '\0', sizeof(src_str));
					strcpy(src_str, src);
					memset(dest_str, '\0', sizeof(dest_str));
					strcpy(dest_str, dest);
					/*chdir("..");*/
				}
				else  /* copy common files */
				{
					strcat(src_str, "/");
					strcat(src_str, entry->d_name);

					chdir("..");  /* before wo copy files, wo should go back to previous
				   					 directory, so that we can use relative path.
									 otherwise, copy_file function can find src_str files
									 or dest_str files*/
					copy_file(src_str, dest_str);
						
					memset(src_str, '\0', sizeof(src_str));
					strcpy(src_str, src);
					return SUCCEED;
				}
			}/* end if */
		}/* end while */
	}
	/*closedir(dir_ptr);*/
	return FAIL;
}


/**
 * @copy_file: copy common a file from src to dest,
 *
 * @src: source file, it can be a file that contain relative path
 * @dest: destination path, it should not contain '/' at then end of the "dest" string
 */
void copy_file(char *src, char *dest)
{
	char *new_file = NULL;
	
	if ((new_file = strrchr(src, '/')) == NULL)
	{
		strcat(dest, "/");
		strcat(dest, src);
	}
	else
	{
		strcat(dest, "/");
		new_file++;
		strcat(dest, new_file);
	}
	
	rename(src, dest);
}
