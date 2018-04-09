#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <utime.h>
#include <dirent.h>
#include  <fcntl.h>
#include "list.h"

char *mergeStrings(char *, char *);
int syncFilesDate(char*, char*);
void loadData(List *, DIR *);

#endif