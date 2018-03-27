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
        if(strcmp(entry->d_name, ".") != 0 || strcmp(entry->d_name, "..") != 0) //xd
        printf("%s -> %d\n", entry->d_name, entry->d_type);
    }
}

void loadData(List * list, DIR * dir)
{
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            if(entry->d_type == 4)
                add(entry->d_name, 1, list);
            else
                add(entry->d_name, 0, list);
        }
    }
}

int main(int argc, char** argv)
{


    char* sourceDirName = argv[1];
    char* destinationDirName = argv[2];

    DIR *source = opendir(sourceDirName);
    DIR *dest = opendir(destinationDirName);

    List * list = emptylist();

    loadData(list, source);
    display(list);
    destroy(list);


    #pragma region  Directory verification
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
    #pragma endregion comment

    #pragma region Linked list testing

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

   // printDirectoryContent(source);

    closedir(source);
    closedir(dest);
    return 0;
}   