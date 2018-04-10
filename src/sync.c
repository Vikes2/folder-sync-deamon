#include "sync.h"



int compare(char* sourceDirPath, char* destinationDirPath, Node* element, List * list);


int compare(char* sourceDirPath, char* destinationDirPath, Node* element, List * list)
{
    //return: 0 the same, 1 diffrent, -1 no file, 2 error
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
            return 0;
        else
            return 1;
    }else
    {
        syslog(LOG_ERR, "Cannot get access to file.");
        return 2;
    }
   
}

int syncFiles(char* sourceDirPath, char* destinationDirPath, size_t sizeTH, int isRecursive)
{
    int ret = 0;

	DIR *source = opendir(sourceDirPath);
    if (source == NULL)
    {
        syslog(LOG_ERR, "Error occurred during opening %s.", sourceDirPath);
        return -1;
    }
    DIR *dest = opendir(destinationDirPath);
    if (dest == NULL)
    {
        syslog(LOG_ERR, "Error occurred during opening %s.", destinationDirPath);
        return -1;
    }

    List * listS = emptylist();
    List * listD = emptylist();

    loadData(listS, source);
    loadData(listD, dest);

    if(listS == NULL && listD == NULL)
        return ret;

    char* pathSource;
    char* pathDestination;

    Node * current;

    while(listS->head != NULL)
    {
        current = popElement(listS);
        int compareStatus = compare(sourceDirPath, destinationDirPath, current, listD);
        if (compareStatus == -1) //nie ma obiektu w dest
        {
            if(current->fileType == 4) // czy Directory
            {

                if(isRecursive == 1)// czy cp wszystko czy tylko files
                {
                    //copy whole Directory
                    ret = copyDirectory(sourceDirPath, destinationDirPath, current->fileName, sizeTH);
                }


            }else                     // zwkly plik cp dla -1
            {
                ret = copyFile(sourceDirPath, destinationDirPath, current->fileName, sizeTH);
            }

        }else if(compareStatus == 1) // obiekty są ale są rozne (1)
        {
            if(current->fileType == 4) // czy Directory
            {
                if(isRecursive == 1)
                {
                    pathSource = mergeStrings(sourceDirPath,current->fileName);
                    pathDestination = mergeStrings(destinationDirPath,current->fileName);
                    ret = syncFiles(pathSource, pathDestination, sizeTH, isRecursive);
                    syncFilesDate(pathSource, pathDestination);
                }
            }
            else
            {
                ret = copyFile(sourceDirPath, destinationDirPath, current->fileName, sizeTH);
            }
        }else if( compareStatus == 2)
            ret = -1;

        deleteElement(current->fileName,listD);
        free(current);
    }

    ret = removeWholeList(destinationDirPath, listD);

    destroy(listS);
    destroy(listD);

    closedir(source);
    closedir(dest);

    return ret;
}