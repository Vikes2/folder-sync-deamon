#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
<<<<<<< HEAD
#include "list.h"

=======
#include <string.h>
#include "list.h"
>>>>>>> 5cc2fdc0ca9de7c0dd697e70ec3acbc943d9a9e0

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
<<<<<<< HEAD

    #pragma region list testing    #pragma region list testing

    // List * list = emptylist();
    // add("aaa",1,list);
    // add("bbb",2,list);
    // add("ccc",3,list);
    // add("ddd",4,list);
    // add("eee",5,list);
    // add("fff",6,list);
    
    // display(list);
    // int find = valueExists("bbb",list);
    // printf("\n%d",find);
    // find = valueExists("bbs",list);
    // printf("\n%d",find);
    // destroy(list);
    #pragma endregion
=======
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
>>>>>>> 5cc2fdc0ca9de7c0dd697e70ec3acbc943d9a9e0

    closedir(source);
    closedir(dest);
    return 0;
}   