#
 ####################################################################################
 # Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ####################################################################################
 # Filename     :  mainWindow.c
 # Version      :  1.0
 # Author       :  Jason Zhong 
 # Created      :  Tue, March 6, 2012 12:19:02 HKT
 # Description  :  Makefile for compile gtk program
 #
 # History      :
 # Revision     :  none
 # Compiler     :  gcc
 #
 #####################################################################################
#

#Source file
SRC = mainWindow.c shell.c eval.c execute.c process.c valuelib.c redirection.c

#Object file
OBJS = $(SRC:.c=.o)

#Output execution file
PROGRAM = mshell

#Compiler
CC = gcc


CFLAGS = -g -Wall

#Include 
INCLUDE = .

#Libs
LIBS= --libs gtk+-2.0

CFLAGS = `pkg-config --cflags ${LIBS}` -g -Wall

INSTALL_DIR = /usr/bin
SRC_SHELL_DIR = .
SRC_CMD_DIR = ../command

##########################################################
#dependency files (.o)
all:${PROGRAM}

${PROGRAM}:${OBJS}
	${CC} -o ${PROGRAM} ${OBJS} ${CFLAGS} -I $(INCLUDE)

${OBJS}:%.o:%.c 
	${CC} -c ${CFLAGS} $< -o $@


.PHONY:clean
clean:
	-rm *.o


r: clean all

install:
	cp ${SRC_SHELL_DIR}/mshell ${INSTALL_DIR}
	cp ${SRC_CMD_DIR}/ls/ils ${INSTALL_DIR}
	cp ${SRC_CMD_DIR}/cat/icat ${INSTALL_DIR}
	cp ${SRC_CMD_DIR}/mkdir/imkdir ${INSTALL_DIR}
	cp ${SRC_CMD_DIR}/more/imore ${INSTALL_DIR}
	cp ${SRC_CMD_DIR}/mv/imv ${INSTALL_DIR}
	cp ${SRC_CMD_DIR}/proc/iproc ${INSTALL_DIR}
	cp ${SRC_CMD_DIR}/pwd/ipwd ${INSTALL_DIR}
	cp ${SRC_CMD_DIR}/rm/irm ${INSTALL_DIR}
	cp ${SRC_CMD_DIR}/who/iwho ${INSTALL_DIR}
