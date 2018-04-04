#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "list.h"
#include "copy.h"
#include "sync.h"

int initParams(char* source, char* destination, int* time, size_t* size, int* isRecursive);

void printDirectoryContent(DIR *dir)
{
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 || strcmp(entry->d_name, "..") != 0) //xd
        printf("%s -> %d\n", entry->d_name, entry->d_type);
    }
}



int initParams(char* source, char* destination, int* time, size_t* size, int* isRecursive)
{
    return 1;
}

void fallAsleep(int time)
{
    return;
}

int main(int argc, char** argv)
{


    char* sourceDirPath = argv[1];
    char* destinationDirPath = argv[2];
    int time = 300;
    size_t sizeTH = 1073741824;
    int isRecursive = 0;

    if(initParams(sourceDirPath, destinationDirPath, &time, &sizeTH, &isRecursive) == 0)
    {
        return 1;
    }

    for(;;)
    {
        fallAsleep(time);
        if(isRecursive=0)
        {
            if(syncFiles(sourceDirPath, destinationDirPath, sizeTH) == 0)
            {
                return 1;
            }
        }
        else
        {
            if(syncRecursive(sourceDirPath, destinationDirPath, sizeTH) == -1)
            {
                return 1;
            }        
        }
    }


    /*
    DIR *source = opendir(sourceDirName);
    DIR *dest = opendir(destinationDirName);

    List * list = emptylist();

    loadData(list, source);
    display(list);
    destroy(list);
    */

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

    //closedir(source);
    //closedir(dest);
    return 0;
}   