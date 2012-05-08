/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename     :  more.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Saturday, May 05, 2012 03:47:57 HKT
 * Description  :  
 *
 * History      :
 * Revision     :  none
 * Compiler     :  gcc
 *
 *************************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>

#include  <unistd.h>
#include   <string.h>
#include   <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>


#define LINELEN 512 /*The length of Line to be printed */  

void do_more(FILE *);  
int see_more(FILE *);  
void noneprint(int); /* To control the terminal echo */  
static void sig_winch(int);  
static void pr_winsize(int);  

struct winsize size;  

int main ( int argc, char *argv[] )  
{  
	FILE *fp; /* File Descriptor */  
	if(signal(SIGWINCH,sig_winch)==SIG_ERR)  
		perror("Signal Error");  
	pr_winsize(1);  
	if ( argc ==1 ) /* If no files exist,use the keyboard */  
		do_more(stdin);   
	/* Through this method,a program may support pipe '|' */  
	else  
		while(--argc)  
			if((fp=fopen(*++argv,"r"))!=NULL)  
			{  
				do_more(fp);  
				fclose(fp);  
			}  
			else  
				exit(1);  
	return EXIT_SUCCESS;  
} /* ---------- end of function main ---------- */  

void do_more(FILE *fp)  
{  
	char line[LINELEN];  
	int num_of_lines=0;  
	int reply;  
	FILE *fp_tty;  
	fp_tty = fopen( "/dev/tty" , "r");  
	if (fp_tty==NULL)  
		exit(1);  

	while(fgets( line,LINELEN,fp))  
	{  
		if( num_of_lines == (size.ws_row-2))  
		{  
			reply = see_more(fp_tty);  
			noneprint(0);  
			if( reply == 0)  
				break;  
			num_of_lines-=reply;  
		}  

		if( fputs (line,stdout)==EOF)  
			exit(1);  
		num_of_lines++;  
	}  

}  

int see_more(FILE *cmd)  
{  
	int c;  
	system ("stty -F /dev/tty cbreak");/*打开/dev/tty作为输入终端，并且控制属性为不需要回车*/  
	printf("/033[7m more? /033[m");  
	noneprint(1);  
	while((c=getc(cmd))!=EOF)  
	{  
		if(c == 'q')  
		{  
			printf("/n");  
			return 0;  
		}   
		if(c == ' ')  
		{  
			//printf("/033[2J");   
			printf("/n");  
			return size.ws_row;  
		}  
		if(c == '\n')  
		{  
			printf("/33[7D/33[K");   
			return 1;  
		}  
	}  
	system ("stty -F /dev/tty -cbreak");/*恢复终端属性为需要回车*/  
	return 0;  
}  

void noneprint(int flag)  
{  
	struct termios init_setting;  
	struct termios pend_setting;  

	if (tcgetattr(1, &init_setting) < 0)  
	{  
		perror("Getting the attribute Error!");  
		exit (1);  
	}  
	pend_setting = init_setting;  
	if(flag==1)  
		pend_setting.c_lflag &= ~ECHO;  
	else  
		pend_setting.c_lflag |= ECHO;  
	tcsetattr (1, TCSANOW, &pend_setting);  
}  

static void pr_winsize(int fd)  
{  
	if(ioctl(fd,TIOCGWINSZ, (char *)&size)<0)  
		perror("TIOCGWINSZ Error!");  
}  

static void sig_winch(int signo)  
{  
	pr_winsize(1);  
}   

