/*
 ************************************************************************************
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ************************************************************************************
 * Filename     :  com.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Monday, April 30, 2012 07:11:05 HKT
 * Description  :  List  information  about  the FILEs (the current directory by default).
				   Sort entries alphabetically if none of -cftuvSUX nor --sort.
 * 				   ils -a, ils, ils 'directory'
 * History      :  add: ils "directory"
 * Revision     :  BUG: 
 * Compiler     :  gcc
 *
 *************************************************************************************
 */

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>

void do_ls(const char dirname[], int flag);
void do_stat(char *);
void show_file_info( char *, struct stat *);
void mode_to_letters( int , char [] );
char *uid_to_name( uid_t );
char *gid_to_name( gid_t );

int main(int argc, const char *argv[])
{
	int i = 0;
	int flag = 0;   /* mark wheather to print detials */
	/*struct dirent *dir_p;*/

	if ( argc == 1 )
	{
		do_ls( ".", 0);
		return 0;
	}
	/* has "-a" or not */
	while (argv[i] != NULL)
	{
		if (strcmp(argv[i++], "-a") == 0) 
		{
			flag = 1;
			break;
		}
	}

	/* "ils -a" */
	if (argc == 2 && flag == 1) 
	{
		do_ls( ".", 1);
		return 0;
	}

	while ( --argc )
	{
		do_ls( *(++argv), flag);
	}
	return 0;
}



/**
 * @do_ls: list dirctory "dirname"
 *
 * @dirname[]: directory name
 * @flag: wheather to list detials,
 * 		  1-yes, 0-no
 */
void do_ls(const char dirname[], int flag)
{
	DIR	  *dir_ptr;
	struct dirent	*direntp;		/* each entry	 */
	/*struct stat     state_buf;*/
	
	if (strcmp(dirname, "-a") == 0) 
	{
		return;
	}

	if ( ( dir_ptr = opendir(dirname) ) == NULL )
	{
		fprintf(stderr,"ils: cannot open %s\n", dirname);
	}
	else
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL )
		{
			if (flag == 1)
			{
				do_stat(direntp->d_name);
			}
			else
			{
				printf("%s\n", direntp->d_name);
			}
		}
		closedir(dir_ptr);
	}
}


/**
 * @do_stat: list filename(or directories) detials
 *
 * @filename: a filename or a directory
 */
void do_stat( char *filename)
{
	struct stat info;

	if ( stat(filename, &info) == -1 )
	{
		perror( filename );			
	}
	else					/* show info */
	{
		show_file_info( filename, &info );
	}
}

/**
 * @show_file_info: display the info about 'filename'. 
 * 					The info is stored in struct at *info_p
 *  		
 * @filename: which file to show(it should not be a directory)
 */
void show_file_info( char *filename, struct stat *info_p )
{
	char	*uid_to_name();
	char	*ctime();
	char	*gid_to_name();
	char    *filemode();
	void	mode_to_letters();
	char    modestr[11];

	mode_to_letters( info_p->st_mode, modestr );

	printf( "%s"    , modestr );
	printf( "%4d "  , (int) info_p->st_nlink);	
	printf( "%-8s " , uid_to_name(info_p->st_uid) );
	printf( "%-8s " , gid_to_name(info_p->st_gid) );
	printf( "%8ld " , (long)info_p->st_size);
	printf( "%.12s ", 4+ctime(&info_p->st_mtime));
	printf( "%s\n"  , filename );
}

/*
 * @mode_to_letters: This function takes a mode value and a char array
 *  				 and puts into the char array the file type and the 
 * 					 nine letters that correspond to the bits in mode.
 * Note: It does not code setuid, setgid, and sticky codes
 * @mode: mode open by opendir.
 */
void mode_to_letters( int mode, char str[] )
{
	strcpy(str, "----------");           /* default=no perms */

	if ( S_ISDIR(mode) )  str[0] = 'd';    /* directory?       */
	if ( S_ISCHR(mode) )  str[0] = 'c';    /* char devices     */
	if ( S_ISBLK(mode) )  str[0] = 'b';    /* block device     */

	if ( mode & S_IRUSR ) str[1] = 'r';    /* 3 bits for user  */
	if ( mode & S_IWUSR ) str[2] = 'w';
	if ( mode & S_IXUSR ) str[3] = 'x';

	if ( mode & S_IRGRP ) str[4] = 'r';    /* 3 bits for group */
	if ( mode & S_IWGRP ) str[5] = 'w';
	if ( mode & S_IXGRP ) str[6] = 'x';

	if ( mode & S_IROTH ) str[7] = 'r';    /* 3 bits for other */
	if ( mode & S_IWOTH ) str[8] = 'w';
	if ( mode & S_IXOTH ) str[9] = 'x';
}


/**
 * @uid_to_name: get name base on uid
 *@return: pointer to username associated with uid, uses getpw()
 */
char *uid_to_name(uid_t uid)
{
	struct	passwd   *getpwuid();
	struct	passwd   *pw_ptr;
	static  char 	 numstr[10];

	if (( pw_ptr = getpwuid( uid )) == NULL)
	{
		sprintf(numstr,"%d", uid);
		return numstr;
	}
	else
	{
		return pw_ptr->pw_name;
	}

	return NULL;
}


/*
 *	returns pointer to group number gid. used getgrgid(3)
 */
char *gid_to_name( gid_t gid )
{
	struct group  *getgrgid();
	struct group  *grp_ptr;
	static char   numstr[10];

	if ( ( grp_ptr = getgrgid(gid) ) == NULL )
	{
		sprintf(numstr,"%d", gid);
		return numstr;
	}
	else
	{
		return grp_ptr->gr_name;
	}
	return NULL;
}