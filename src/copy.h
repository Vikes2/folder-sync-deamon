#ifndef COPY_H
#define COPY_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "sync.h"

int copyFile(char*, char*);
int copyDirectory(char *, char *);

#endif