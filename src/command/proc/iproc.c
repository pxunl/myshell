/*
 ************************************************************************************
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ************************************************************************************
 * Filename     :  iproc.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Tuesday, May 08, 2012 04:54:50 HKT
 * Description  :  The  proc  file  system is a pseudo-file system which is used as an
 *      		   interface to kernel data structures.  It  is  commonly  mounted  at
 *     			   /proc.
 *     			   iproc will read information about current system from the file "/proc"
 *     			   -c(--cpu: information about cpu
 *     			   -m(--meninfo
 *     			   -M(--mount
 *     			   -s(--swap:
 *     			   -v(--version: version of kernel
 *     			   -u(--uptime: run time
 *     			   -d(--devices: availabe devices
 *
 *
 * History      :
 * Revision     :  none
 * Compiler     :  gcc
 *
 *************************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define program_name   "iproc"

#define R_TRUE         0

#define R_FALSE        1

#define NUM_CMD        7

#define BUF_SIZE       64

void info_cpu();     
void info_mem();
void info_mount();
void info_swap();
void info_kernel_version();
void info_uptime();
void info_devices();

void (*action_table[NUM_CMD])() = {info_cpu, info_mem,
									  info_mount, info_swap,
									  info_kernel_version,
									  info_uptime, info_devices};
/* 
 *     			   -c(--cpu: information about cpu
 *     			   -m(--meninfo
 *     			   -M(--mount
 *     			   -s(--swap:
 *     			   -v(--version: version of kernel
 *     			   -u(--uptime: run time
 *     			   -d(--devices: availabe devices
 */
char *cmd_table[NUM_CMD] = {"-c", "-m", "-M", "-s", "-v", "-u", "-d"};

void usage(int status);


int main(int argc, const char *argv[])
{
	if (argc < 2) 
	{
		usage(R_TRUE);
	}
	
	int i;
	int j;
	for (i = 1; i < argc; i++) 
	{
		for (j = 0; j < NUM_CMD; j++) 
		{
			if (strcmp(argv[i], cmd_table[j]) == 0) 
			{
				action_table[j]();
			}
		}
	}
	return 0;
}

void usage(int status)
{
	if (status != R_TRUE)
		fprintf (stderr, ("Try `%s --help' for more information.\n"),
				program_name);
	else
	{
		printf (("Usage: %s [OPTION]...\n"), program_name);
		printf(("%s\n%s\n%s\n%s\n%s\n%s\n%s\n"), "\nprint information about current system\n",
					"\t-c(--cpu: information about cpu",
					"\t-m(--meninfo",
					"\t-M(--mount -s(--swap",
					"\t-v(--version: version of kernel",
					"\t-u(--uptime: run time",
					"\t-d(--devices: availabe devices");
   	}
	exit (status);
}

/**
 * @info_cpu: -c(--cpu: information about cpu
 */
void info_cpu()
{
	int read_size = 0;
	char buf[BUF_SIZE];
	int fd;
	if ((fd = open("/proc/cpuinfo", O_RDONLY)) == -1) 
	{
		perror("can't read /proc/cpuinfo, please check it's permision\n");
		exit(1);
	}
	
	while ((read_size = read(fd, buf, BUF_SIZE)) != 0)
	{
		buf[read_size] = '\0';
		printf("%s", buf);
	}
	close(fd);
	/*system("cat /proc/cpuinfo");*/
}

/**
 * @info_mem:-m(--meninfo
 */
void info_mem()
{
	int read_size = 0;
	char buf[BUF_SIZE];
	int fd;
	if ((fd = open("/proc/meminfo", O_RDONLY)) == -1) 
	{
		perror("can't read /proc/meminfo, please check it's permision\n");
		exit(1);
	}
	
	while ((read_size = read(fd, buf, BUF_SIZE)) != 0)
	{
		buf[read_size] = '\0';
		printf("%s", buf);
	}
	close(fd);
}

/**
 * @info_mount: -M(--mount
 */
void info_mount()
{
	int  read_size = 0;
	char buf[BUF_SIZE];
	int fd;
	if ((fd = open("/proc/mounts", O_RDONLY)) == -1) 
	{
		perror("can't read /proc/mounts, please check it's permision\n");
		exit(1);
	}
	
	while ((read_size = read(fd, buf, BUF_SIZE)) != 0)
	{
		buf[read_size] = '\0';
		printf("%s", buf);
	}
	close(fd);
}

/**
 * @info_swap: -s(--swap:
 */
void info_swap()
{
	int  read_size = 0;
	char buf[BUF_SIZE];
	int fd;
	if ((fd = open("/proc/swaps", O_RDONLY)) == -1) 
	{
		perror("can't read /proc/swaps, please check it's permision\n");
		exit(1);
	}
	
	while ((read_size = read(fd, buf, BUF_SIZE)) != 0)
	{
		buf[read_size] = '\0';
		printf("%s", buf);
	}
	close(fd);
}

/**
 * @info_kernel_version:-v(--version: version of kernel
 */
void info_kernel_version()
{
	int  read_size = 0;
	char buf[BUF_SIZE];
	int fd;
	if ((fd = open("/proc/version", O_RDONLY)) == -1) 
	{
		perror("can't read /proc/version, please check it's permision\n");
		exit(1);
	}
	
	while ((read_size = read(fd, buf, BUF_SIZE)) != 0)
	{
		buf[read_size] = '\0';
		printf("%s", buf);
	}
	close(fd);
}

/**
 * @info_uptime: -u(--version: version of kernel
 */
void info_uptime()
{
	int  read_size = 0;
	char buf[BUF_SIZE];
	int fd;
	if ((fd = open("/proc/uptime", O_RDONLY)) == -1) 
	{
		perror("can't read /proc/uptime, please check it's permision\n");
		exit(1);
	}
	
	while ((read_size = read(fd, buf, BUF_SIZE)) != 0)
	{
		buf[read_size] = '\0';
		printf("%s", buf);
	}
	close(fd);
}

/**
 * @info_devices: -d(--devices: availabe devices
 */
void info_devices()
{
	int  read_size = 0;
	char buf[BUF_SIZE];
	int fd;
	if ((fd = open("/proc/devices", O_RDONLY)) == -1) 
	{
		perror("can't read /proc/devices, please check it's permision\n");
		exit(1);
	}
	
	while ((read_size = read(fd, buf, BUF_SIZE)) != 0)
	{
		buf[read_size] = '\0';
		printf("%s", buf);
	}
	close(fd);
}
