#ifndef COPY_H
#define COPY_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/mman.h>
#include "util.h"

int copyFile(char *, char *, char *, int);
int mmapCopyFile(char *, char *);
int standardCopyFile(char *, char *);
int copyDirectory(char *, char *, char *, int);

#endif