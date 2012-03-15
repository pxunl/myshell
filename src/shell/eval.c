/**
 *eval.c
 *contain function spiltline read_command ...
 *2012-3-14
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "eval.h"
#include "config.h"

#define is_none_c(x) ((x)==' ' || (x)=='\t')


/**
 * @fatla error
 *
 * @str_err: error information
 * @str_error: error information
 * @exit_code: exit code
 */
void fatal(char *str_err, char *str_error, int exit_code)
{
	fprintf(stderr, "Error: %s,%s\n", str_err, str_error);
	exit(exit_code);
}

void *emalloc(size_t n)
{
	void *rv;
	if ((rv = malloc(n)) == NULL)
		fatal("out of memory!\n", "", 1);
	return rv;
}

void *erealloc(void *point, size_t n)
{
	void *rv;
	if ((rv = realloc(point, n)) == NULL)	
	{
		fatal("realloc() fail!", "" , 1);
	}

	return rv;
}

/**
 * @read a new command line from FILE stream fp
 *
 * @fp standard input
 * @return a point of the new command line
 */
char * read_command(FILE *fp)
{
	char *buf;
	int buf_space = 0;
	int posion = 0;
	int c;

	printf("%s\n", DEF_PROMPT);
	while ((c = getc(fp)) != EOF) 
	{	
		/*no enough space*/
		if (posion+1 >= buf_space) 
		{
			if (buf_space == 0) 
			{
				buf = (char*)emalloc(BUFSIZE);
			}
			else 
			{
				buf = (char*)erealloc(buf, buf_space+BUFSIZE);
			}
			buf_space += BUFSIZE;
		}

		/*end of command*/
		if (c == '\n') 
			break;

		/*if this is not the end*/
		buf[posion++] = c;
	}
	
	if (c == EOF && posion == 0) 
	{
		return NULL;
	}

	buf[posion] = '\0';
	return buf;
}


/**
 * @create a string to store the command word
 *
 * @begin: start of the word
 * @len: the length of the word
 * @return: command word 
 */
char * create_str(char *begin, int len)
{
	char *new_str = emalloc(len+1);

	new_str[len] = '\0';
	strncpy(new_str, begin, len);
	return new_str;
}



/**
 * @spilt the the input string to command and put them in array
 *
 * @cmdline: input string from stream
 * @return : array of command line
 */
char **spiltline(char *cmdline)
{
	char **args;
	int pots = 0;
	int buf_num = 0;
	int arg_num = 0;
	char *str= cmdline;
	char *start;
	int length;

	if (cmdline == NULL) 
	{
		return NULL;	
	}

	args = emalloc(BUFSIZE);
	buf_num = BUFSIZE;
	pots = BUFSIZE/sizeof(char *);

	while (*str != '\0') 
	{
		/*take away the usefull head chars*/
		while (is_none_c(*str)) 
			str++;

		if (*str == '\0') 
			break;
	
		/*no enough space in args*/
		if (arg_num+1 >= pots) 
		{
			args = erealloc(args, buf_num+BUFSIZE);
			buf_num += BUFSIZE;
			pots += (BUFSIZE/sizeof(char*));
		}

		start = str;
		length = 1;
		while (*++str != '\0' && !is_none_c(*str)) 
			length++;

		args[arg_num++] = create_str(start, length);

		str++; /*next char*/
	}

	args[arg_num] = NULL;
	return args;
}


void free_list(char **list)
{
	char **cp = list;
	while (*cp)
		free(*cp++);
	free(list);
}
