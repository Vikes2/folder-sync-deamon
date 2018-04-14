#include "sync.h"

int compare(char* sourceDirPath, char* destinationDirPath, Node* element, List * list)
{
    //return: 0 the same, 1 diffrent, -1 no file, 2 error
    if(valueExists(element->fileName,element->fileType,list) == 0)
    {
        return -1;
    }
    int ret;
    struct stat sb;
    struct stat db;
    char* pathSource = joinToPath(sourceDirPath, element->fileName);
    char* pathDestination = joinToPath(destinationDirPath, element->fileName);

    if(stat(pathSource,&sb) == 0 && stat(pathDestination,&db) == 0)
    {
        if(sb.st_mtime == db.st_mtime)
            ret = 0;
        else
            ret = 1;
    }
    else
    {
        syslog(LOG_ERR, "stat() %s", strerror(errno));
        ret = 2;
    }
   
    free(pathSource);
    free(pathDestination);

    return ret;
}

int syncFiles(char *sourceDirPath, char *destinationDirPath, size_t sizeTH, int isRecursive)
{
    int ret = 0;
    DIR *source, *dest;
    List *listS, *listD;
    char *pathSource, *pathDestination;
    Node *current;

    if ((source = opendir(sourceDirPath)) == NULL)
    {
        syslog(LOG_ERR, "Sync failed; opendir() \"%s\" %s.", sourceDirPath, strerror(errno));
        return -1;
    }

    if ((dest = opendir(destinationDirPath)) == NULL)
    {
        syslog(LOG_ERR, "Sync failed; opendir() \"%s\" %s.", destinationDirPath, strerror(errno));
        return -1;
    }

    listS = emptyList();
    listD = emptyList();

    loadData(listS, source);
    loadData(listD, dest);

    if(listS->head == NULL && listD->head == NULL)
        return ret;

    while(listS->head != NULL)
    {
        current = popElement(listS);
        int compareStatus = compare(sourceDirPath, destinationDirPath, current, listD);
        if (compareStatus == -1) //nie ma obiektu w dest
        {
            if(current->fileType == 4 && isRecursive == 1) // czy Directory
            {
                ret = copyDirectory(sourceDirPath, destinationDirPath, current->fileName, sizeTH);
            }
            else                     // zwkly plik cp dla -1
            {
                ret = copyFile(sourceDirPath, destinationDirPath, current->fileName, sizeTH);
            }

        }
        else if(compareStatus == 1) // obiekty są ale są rozne (1)
        {
            if(current->fileType == 4 && isRecursive == 1) // czy Directory
            {
                pathSource = joinToPath(sourceDirPath,current->fileName);
                pathDestination = joinToPath(destinationDirPath,current->fileName);
                ret = syncFiles(pathSource, pathDestination, sizeTH, isRecursive);
                syncFilesDate(pathSource, pathDestination);
                free(pathSource);
                free(pathDestination);
            }
            else
            {
                ret = copyFile(sourceDirPath, destinationDirPath, current->fileName, sizeTH);
            }
        }
        else if(compareStatus == 2)
            ret = -1;

        deleteElement(current->fileName,listD);
        free(current);
    }

    ret = removeWholeList(destinationDirPath, listD);

    destroy(&listS);
    destroy(&listD);

    closedir(source);
    closedir(dest);

    return ret;
}