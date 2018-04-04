#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int syncFiles(char* sourceDirPath, char* destinationDirPath, size_t sizeTH);
int syncRecursive(char* sourceDirPath, char* destinationDirPath, size_t sizeTH);