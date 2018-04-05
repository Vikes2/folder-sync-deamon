#include "sync.h"


void loadData(List * list, DIR * dir);
int compare(char* sourceDirPath, char* destinationDirPath, Node* element, List * list);
  
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
                add(entry->d_name, 0, list);//hh
        }
    }
}


int compare(char* sourceDirPath, char* destinationDirPath, Node* element, List * list)
{
    //return: 0 the same, 1 diffrent
    if(valueExists(element->fileName,element->fileType,list) ==0)
    {
        return 1;
    }
    struct stat sb;
    struct stat db;
    char* pathSource = (char *) malloc(1+strlen(sourceDirPath) + strlen(element->fileName));
    char* pathDestination = (char *) malloc(1+strlen(destinationDirPath) + strlen(element->fileName));
    strcpy(pathSource,sourceDirPath);
    strcpy(pathDestination,sourceDirPath);
    strcat(pathSource,element->fileName);
    strcat(pathDestination,element->fileName);


    if( stat(pathSource,&sb) == 0 && stat(pathDestination,&db) == 0)
    {
        // sb.st_mtime:time_t
        if(sb.st_mtime == db.st_mtime)
        {
            return 0;
        }else
        {
            return 1;
        }

    }else
    {
        // to think about it
        return 0;
        printf("stat went wrong");
    }
   
}

int syncFiles(char* sourceDirPath, char* destinationDirPath, size_t sizeTH)
{
	DIR *source = opendir(sourceDirPath);
    DIR *dest = opendir(destinationDirPath);

    List * listS = emptylist();
    List * listD = emptylist();

    loadData(listS, source);
    loadData(listD, dest);

    display(listS);
    display(listD);
    if(compare(sourceDirPath,destinationDirPath, popElement(listS),listD) == 0)
        printf("\ntakie same");
    else
        printf("\nnie=");




    destroy(listS);
    destroy(listD);

    closedir(source);
    closedir(dest);

    return 0;
}

int syncRecursive(char* sourceDirPath, char* destinationDirPath, size_t sizeTH)
{

}