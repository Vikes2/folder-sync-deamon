#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "list.h"


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

    return 0;
}   