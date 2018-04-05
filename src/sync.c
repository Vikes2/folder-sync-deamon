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

          add(entry->d_name, entry->d_type, list);//hh
        }
    }
}

char* mergeStrings(char* string1, char* string2)
{
    char* result = (char *) malloc(2+ strlen(string1) + strlen(string2));
    strcpy(result,string1);
    strcat(result,"/");
    strcat(result,string2);
    printf("%s",result);
    return result;
}

int compare(char* sourceDirPath, char* destinationDirPath, Node* element, List * list)
{
    //return: 0 the same, 1 diffrent, -1 no file
    if(valueExists(element->fileName,element->fileType,list) ==0)
    {
        return 1;
    }
    struct stat sb;
    struct stat db;
    char* pathSource = mergeStrings(sourceDirPath,element->fileName);
    char* pathDestination = mergeStrings(destinationDirPath,element->fileName);

    if(stat(pathSource,&sb) == 0)
    {
        if(stat(pathDestination,&db) == 0)
        {
            if(sb.st_mtime == db.st_mtime)
            {
                return 0;
            }else
            {
                return 1;
            }
        }
        else
        {
            if(errno == ENOENT)
            {
                // nie ma pliku w dest
                return -1;
            }
        }
    }
    else
    {
        exit(EXIT_FAILURE);
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

    if(listS == NULL && listD == NULL)
        return 0;

    display(listS);
    printf("\n");
    display(listD);

    Node * current;

    while(listS != NULL)
    {
        current = popElement(listS);
        int compareStatus = compare(sourceDirPath,destinationDirPath,current, listD);
        if( compareStatus == 0)
        {
            //object the same

            deleteElement(current->fileName,listD);
            continue;
        }else if (compareStatus == -1)
        {
            //nie ma pliku w dest
            if(current->fileType == 4) //if(directory)
            {
                if(1)//if(isRecursive)
                {
                    //copy all
                }else
                {
                    continue;
                }

            }else
            {
                //copy file
            }

        }else
        {
            //pliki sa rozne
            printf("\nnie=");
        }
    }



   


    destroy(listS);
    destroy(listD);

    closedir(source);
    closedir(dest);

    return 0;
}

int syncRecursive(char* sourceDirPath, char* destinationDirPath, size_t sizeTH)
{

}