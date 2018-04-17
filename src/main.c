#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>
#include <linux/fs.h>
#include "list.h"
#include "sync.h"
#include "copy.h"

int autod = 1;

int initParams(int argc, char** argv, char** source, char** destination, int* time, size_t* size, int* isRecursive);

int initParams(int argc, char** argv, char** source, char** destination, int* time, size_t* size, int* isRecursive)
{
    int opt;

    DIR *dir;

    *source = argv[1];
    *destination = argv[2];

    if(argc < 3) //at least 3 arguments are necessary, e.g. ./sync source destination 
    {
        fprintf(stderr, "Usage: %s source destination [-t secs] [-s size] [-R]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if((dir = opendir(*source)) == NULL)
    {
        if(errno == ENOTDIR){
            fprintf(stderr, "%s is not a directory\n", *source);
            exit(EXIT_FAILURE);
        }

        fprintf(stderr, "%s is not a valid directory\n", *source);
        exit(EXIT_FAILURE);
    }

    closedir(dir);

    if(( dir = opendir(*destination)) == NULL)
    {
        if(errno == ENOTDIR){
            fprintf(stderr, "%s is not a directory\n", *destination);
            exit(EXIT_FAILURE);
        }

        fprintf(stderr, "%s is not a valid directory\n", *destination);
        exit(EXIT_FAILURE);
    }

    closedir(dir);

    if(strcmp(*source, *destination) == 0) // source and destination have to be different directories
    {
        fprintf(stderr, "Source and destination paths are the same\n");
        exit(EXIT_FAILURE);
    }

    while ((opt = getopt(argc, argv, "t:s:R")) != -1)
    {
        switch(opt)
        {
            case 't':
                *time = atoi(optarg);
                break;
            case 's':
                *size = atoi(optarg);
                break;
            case 'R':
                *isRecursive = 1;
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s source destination [-t secs] [-s size] [-R]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    return optind;
}

void signalHandler(int signo) 
{
	switch (signo) {
	case SIGUSR1:
		syslog(LOG_INFO, "Received SIGUSR1. Process awakened by user.");
        autod = 0;
		break;
	case SIGTERM:
		syslog(LOG_INFO, "Received SIGTERM. Process terminated by user.");
		exit(EXIT_SUCCESS);
		break;
	}
}

void deamonize()
{
    int i;
    pid_t pid;

    pid = fork();
    if(pid == -1)
    {
        exit(EXIT_FAILURE);
    }
    else if(pid != 0)
    {
        exit(EXIT_SUCCESS);
    }

    if(setsid() == -1)
    {
        exit(EXIT_FAILURE);
    }

    for(i = 0; i <  sysconf(_SC_OPEN_MAX); i++)
    {
        close(i);
    }

    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);
    chdir("/");

    openlog("sync", LOG_PID, LOG_USER);
}

int main(int argc, char** argv)
{
    int ret;
    char* sourceDirPath;
    char* destinationDirPath;
    int time = 300;
    size_t sizeTh = 1073741824;
    int isRecursive = 0;

    if (signal(SIGUSR1, &signalHandler) == SIG_ERR) {   //  Ustawienie handlera sygnału SIGUSR1
		perror("signal()");
		exit(EXIT_FAILURE);
	}

	if (signal(SIGTERM, &signalHandler) == SIG_ERR) {   //  Ustawienie handlera sygnału SIGTERM
		perror("signal()");
		exit(EXIT_FAILURE);
    }

    if(initParams(argc, argv, &sourceDirPath, &destinationDirPath, &time, &sizeTh, &isRecursive) >= argc)
    {
       fprintf(stderr, "Expected argument after options\n");
       exit(EXIT_FAILURE);
    }
    ret = 0;

    deamonize();
    
    while(1)
    {
        if(autod)
        syslog(LOG_INFO, "Deamon has been started automatically.");

        if(syncFiles(sourceDirPath, destinationDirPath, sizeTh, isRecursive) == -1)
        {
            syslog(LOG_ERR, "Synchronization failed.");
            ret = 1;
            break;
        }

        syslog(LOG_INFO, "Deamon is sleeping for %d seconds...", time);
        autod = 1;
        sleep(time);
    }

    return ret;
}   
