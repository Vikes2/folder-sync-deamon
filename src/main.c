#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "list.h"
#include "sync.h"

int initParams(int argc, char** argv, char* source, char* destination, int* time, size_t* size, int* isRecursive);

void printDirectoryContent(DIR *dir)
{
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 || strcmp(entry->d_name, "..") != 0) //xd
        printf("%s -> %d\n", entry->d_name, entry->d_type);
    }
}



int initParams(int argc, char** argv, char* source, char* destination, int* time, size_t* size, int* isRecursive)
{
    int opt;

    source = argv[1];
    destination = argv[2];

    if(argc < 3) //at least 3 arguments are necessary, e.g. ./sync source destination 
    {
        fprintf(stderr, "Usage: %s source destination [-t secs] [-s size] [-R]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(strcmp(source, destination) == 0) // source and destination have to be different directories
    {
        fprintf(stderr, "Source and destination paths are the same\n");
        exit(EXIT_FAILURE);
    }

    while ((opt = getopt(argc, argv, "t:s:R")) != -1)
    {
        switch(opt)
        {
            case 't':
                *time = atoi(optarg);
                break;
            case 's':
                *size = atoi(optarg);
                break;
            case 'R':
                *isRecursive = 1;
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s source destination [-t secs] [-s size] [-R]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    return optind;
}

int main(int argc, char** argv)
{


    char* sourceDirPath = argv[1];
    char* destinationDirPath = argv[2];
    int time = 300;
    size_t sizeTH = 1073741824;
    int isRecursive = 0;

    if(initParams(argc, argv, sourceDirPath, destinationDirPath, &time, &sizeTH, &isRecursive) >= argc)
    {
       fprintf(stderr, "Expected argument after options\n");
       exit(EXIT_FAILURE);
    }

    if(isRecursive == 0)
    {
        if(syncFiles(sourceDirPath, destinationDirPath, sizeTH) == 0)
        {
            return 1;
        }
    }
    else
    {
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

    // printf("\n");
    
    // display(list);
    // printf("\n");
    // deleteElement("ccaa",list);
    // display(list);
    // printf("\n");

    // Node * element ;
    // element = popElement(list);
    // printf("%s ", element->fileName);

    // free(element);

    // display(list);
    // printf("\n");

    #pragma endregion

   // printDirectoryContent(source);

    //closedir(source);
    //closedir(dest);
    return 0;
}   
