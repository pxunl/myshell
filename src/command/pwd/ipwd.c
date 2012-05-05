/*
 ************************************************************************************
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ************************************************************************************
 * Filename     :  ipwd.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Monday, April 30, 2012 07:11:05 HKT
 * Description  :  starts in current directory and recursively
 *				   climbs up to root of filesystem, prints top part
 *				   then prints current part
 *				   uses readdir() to get info about each thing
 * History      : 
 * Revision     :  BUG: 
 * Compiler     :  gcc
 *************************************************************************************
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

ino_t	get_inode(char *);
void    printpathto(ino_t);
void    inum_to_name(ino_t , char *, int );

int main(int argc, const char *argv[])
{
	printpathto( get_inode( "." ) );	/* print path to here	*/
	putchar('\n');		
	return 0;
}

/*
 *	prints path leading down to an object with this inode
 *	kindof recursive
 */
void printpathto( ino_t this_inode )
{
	ino_t	my_inode ;
	char	its_name[BUFSIZ];

	if ( get_inode("..") != this_inode )
	{
		chdir( ".." );		

		inum_to_name(this_inode,its_name,BUFSIZ);/* get its name*/

		my_inode = get_inode( "." );		/* print head	*/
		printpathto( my_inode );
		printf("/%s", its_name );	
	}
}

/*
 *	looks through current directory for a file with this inode
 *	number and copies its name into namebuf
 */
void inum_to_name(ino_t inode_to_find , char *namebuf, int buflen)
{
	DIR		*dir_ptr;		
	struct dirent	*direntp;		/* each entry	 */

	dir_ptr = opendir( "." );
	if ( dir_ptr == NULL )
	{
		perror( "." );
		exit(1);
	}

	/* search directory for a file with specified inum */
	while ( ( direntp = readdir( dir_ptr ) ) != NULL )
	{
		if ( direntp->d_ino == inode_to_find )
		{
			strncpy( namebuf, direntp->d_name, buflen);
			namebuf[buflen-1] = '\0';   /* just in case */
			closedir( dir_ptr );
			return;
		}
	}
	fprintf(stderr, "error looking for inum %d\n", (int)inode_to_find);
	exit(1);
}

/*
 *	returns inode number of the file
 */
ino_t get_inode( char *fname )
{
	struct stat info;

	if ( stat( fname , &info ) == -1 )
	{
		fprintf(stderr, "Cannot stat ");
		perror(fname);
		exit(1);
	}
	return info.st_ino;
}
