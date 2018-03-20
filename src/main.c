#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

void printDirectoryContent(const char* path)
{
    struct dirent *entry;
    int ret = 1;
    DIR *dir;

    dir = opendir(path);
    
    if(dir)
    {
        while((entry = readdir(dir)) != NULL)
        {
            printf("%s\n", entry->d_name);
        }
        closedir(dir); 
    }
}

int main(int argc, char** argv)
{
    printDirectoryContent(".");

    return 0;
}