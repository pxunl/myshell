/*
 ************************************************************************************
 * Copyright (c),  2011-2014  dd.pangxie@gmail.com
 ************************************************************************************
 * Filename      :  redirection.h
 * Version       :  1.0
 * Author        :  Jason Zhong
 * Created       :  Tuesday, May 22, 2012 08:40:45 HKT
 * Description   :  functions:
 *
 * History       :
 * Revision		 :  none
 * Compiler      :  gcc
 ************************************************************************************
 */

#ifndef __REDIRECTION_H__
#define __REDIRECTION_H__

enum Redirection_types
{
	Re_singal_out,   /* reidrect '<'  */
	Re_singal_in,    /* redirect '>'  */
	Re_double_out, 	 /* redirect '>>' */
	Re_pipe       	 /* pipe for '|'  */
};


struct REDIRECT
{
	char *word;

#ifdef HAVE_DEEP_DERECTIONS
	int  redirector;   /* Descriptor to be redirected */
	int  dest;
#endif

	enum Redirection_types red_type;
	char *re_file;
	struct REDIRECT *next;
}*Node; 

int Check_Redirection_Validity(char **cmd);
char **get_program(char **cmd);
int is_in_descriptor_table(char * str);
void Usage_Redirection(int status);
int Need_Redirection(char **cmd);
int Process_Redirection(char **cmd);
int Check_File_Name(char *file);

int do_redirect_output(char **program, struct REDIRECT *redirection);
int do_redirect_input(char **program, struct REDIRECT *redirection);
int do_redirect_double_output(char **program, struct REDIRECT *redirection);
int do_redirect_pipe(char **program, struct REDIRECT *redirection);
void Redirection_Usage();
struct REDIRECT *get_redirection_list(char ** cmd);

/* free memory */
void free_direction_list(struct REDIRECT *pfree);
void free_temp_cmd(char **cmd);

#endif
