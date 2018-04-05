#ifndef SYNC_H
#define SYNC_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "list.h"

int syncFiles(char* sourceDirPath, char* destinationDirPath, size_t sizeTH);
int syncRecursive(char* sourceDirPath, char* destinationDirPath, size_t sizeTH);

#endif