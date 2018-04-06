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
          add(entry->d_name, entry->d_type, list);
        }
    }
}

char* mergeStrings(char* string1, char* string2)
{
    char* result = (char *) malloc(2+ strlen(string1) + strlen(string2));
    strcpy(result,string1);
    strcat(result,"/");
    strcat(result,string2);
    //printf("%s",result);
    return result;
}

int compare(char* sourceDirPath, char* destinationDirPath, Node* element, List * list)
{
    //return: 0 the same, 1 diffrent, -1 no file
    if(valueExists(element->fileName,element->fileType,list) == 0)
    {
        return -1;
    }
    struct stat sb;
    struct stat db;
    char* pathSource = mergeStrings(sourceDirPath,element->fileName);
    char* pathDestination = mergeStrings(destinationDirPath,element->fileName);

    if(stat(pathSource,&sb) == 0 && stat(pathDestination,&db) == 0)
    {

        if(sb.st_mtime == db.st_mtime)
        {
            return 0;
        }else
        {
            return 1;
        }
    }else
    {
        exit(EXIT_FAILURE);
    }
   
}

int syncFiles(char* sourceDirPath, char* destinationDirPath, size_t sizeTH, int isRecursive)
{
    int ret = 0;

	DIR *source = opendir(sourceDirPath);
    DIR *dest = opendir(destinationDirPath);

    List * listS = emptylist();
    List * listD = emptylist();

    loadData(listS, source);
    loadData(listD, dest);

    if(listS == NULL && listD == NULL)
        return ret;

    char* pathSource;
    char* pathDestination;

    

    Node * current;

    while(listS != NULL)
    {
        current = popElement(listS);
        int compareStatus = compare(sourceDirPath, destinationDirPath, current, listD);
        if( compareStatus == 0)
        {
            //objects the same
        }else if (compareStatus == -1) //nie ma obiektu w dest
        {
            if(current->fileType == 4) // czy Directory
            {
                if(isRecursive == 1)// czy cp wszystko czy tylko files
                {
                    //copy whole Director
                    if(copyDirectory(sourceDirPath, destinationDirPath, current->fileName, sizeTH) == -1)
                    {
                        // ------------------------------------------------------------------------error
                        ret = -1;
                    }
                }

            }else                     // zwkly plik cp dla -1
            {
                if(copyFile(sourceDirPath, destinationDirPath, current->fileName, sizeTH)== -1)
                {
                    //===============================================================================error add syslog
                    //go next
                    ret = -1;

                }
            }

        }else // obiekty są ale są rozne (1)
        {
            if(current->fileType == 4) // czy Directory
            {
                if(isRecursive == 1)
                {
                    pathSource = mergeStrings(sourceDirPath,current->fileName);
                    pathDestination = mergeStrings(destinationDirPath,current->fileName);
                    if(syncFiles(pathSource, pathDestination, sizeTH, isRecursive) == -1)
                    {
                        //====================================error
                        ret = -1;

                    }
                    //ustawic nowa date modify
                    //
                    //
                    //go next
                }
            }
            else
            {

                if(copyFile(sourceDirPath, destinationDirPath, current->fileName, sizeTH)== -1)
                {
                    //===============================================================error
                    ret = -1;

                }
            }
        }
        deleteElement(current->fileName,listD);
        free(current);

    }

    destroy(listS);
    destroy(listD);

    closedir(source);
    closedir(dest);

    return ret;
}

int syncRecursive(char* sourceDirPath, char* destinationDirPath, size_t sizeTH)
{

}
