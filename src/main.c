#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "list.h"

void printDirectoryContent(DIR *dir)
{
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 || strcmp(entry->d_name, "..") != 0)
        printf("%s -> %d\n", entry->d_name, entry->d_type);
    }
}

int main(int argc, char** argv)
{
    char* sourceDirName = argv[1];
    char* destinationDirName = argv[2];

    DIR *source = opendir(sourceDirName);
    DIR *dest = opendir(destinationDirName);

    /**
     
     * Weryfikacja katalogów
    
    if(!source)
    {
        perror(sourceDirName);
        return 1;
    }

    if(!dest)
    {
        perror(destinationDirName);
        return 1;
    }

    */

    printDirectoryContent(source);

    closedir(source);
    closedir(dest);
    return 0;
}