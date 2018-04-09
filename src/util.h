#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <utime.h>

char *mergeStrings(char *, char *);
int syncFilesDate(char*, char*);

#endif