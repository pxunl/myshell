#
#Author:PXunL
#dd.pangxie@gmil.com
#2012-3-6
#Makefile for compile gtk program


#Source file
SRC =  shell.c eval.c

#Object file
OBJS = $(SRC:.c=.o)

#Output execution file
PROGRAM = shell

#Compiler
CC = gcc



CFLAGS = -g -Wall



##########################################################
#dependency files (.o)
all:${PROGRAM}

${PROGRAM}:${OBJS}
	${CC} -o ${PROGRAM} ${OBJS} ${CFLAGS}

${OBJS}:%.o:%.c
	${CC} -c ${CFLAGS} $< -o $@



.PHONY:clean
clean:
	-rm *.o

r: clean all
