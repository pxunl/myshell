/*
 * eval.h
 *head file for eval.c
 *contain function spiltline read_commad and so on
 *
 */


#define BUFSIZE 128

char * create_str(char *begin, int len);

char **spiltline(char *cmdline);

char * read_command(FILE *fp);

void fatal(char *str_err, char *str_error, int exit_code);

void free_list(char **list);
