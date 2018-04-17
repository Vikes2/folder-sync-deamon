#include "util.h"

char *joinToPath(char *string1, char *string2)
{
    char *result = (char *) malloc(2+ strlen(string1) + strlen(string2));
    strcpy(result,string1);
    strcat(result,"/");
    strcat(result,string2);
    return result;
}

int syncFilesDate(char *sourcePath, char *destPath)
{
    struct stat sb;
    struct utimbuf newTime;

    stat(sourcePath, &sb);

    newTime.actime = time(NULL);
	newTime.modtime = sb.st_mtime;
	if (utime(destPath, &newTime) == -1) 
    {
        syslog(LOG_ERR, "utime(): \"%s\" %s", sourcePath, strerror(errno));        
		return -1;
	}

    return 0;
}

void loadData(List * list, DIR * dir)
{
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0
        && (entry->d_type == 4 ||entry->d_type == 8)        )
        {
          add(entry->d_name, entry->d_type, list);
        }
    }
}


int removeWholeList(char *path, List * list, int isRecursive)
{
    Node *current;
    DIR *next;
    List *subList;
    char *fullPath;
    int ret = 0;
    while(list->head != NULL)
    {
        current = popElement(list);
        fullPath= joinToPath(path, current->fileName);
        if(current->fileType != 4)
        {
            if(remove(fullPath) == -1)
            {
                syslog(LOG_ERR, "Deleting %s failed: Error occurred during deleting.", fullPath);
                ret = -1;
            }else
                syslog(LOG_INFO, "Deleting %s succeed.", fullPath);
        }
        else
        {
            if(isRecursive == 1)
            {
                if ((next = opendir(fullPath)) == NULL)
            {
                syslog(LOG_ERR, "Deleting %s failed: Error occurred during opening %s.", fullPath, fullPath);
                free(current);
                free(fullPath);
                return -1;
            }
            subList = emptyList();
            loadData(subList, next);
            ret = removeWholeList(fullPath, subList, isRecursive);
            if(remove(fullPath) == -1)
            {
                syslog(LOG_ERR, "Deleting %s failed: Error occurred during deleting.", fullPath);
                ret = -1;
            }else
                syslog(LOG_INFO, "Deleting %s succeed.", fullPath);
            destroy(&subList);
            closedir(next);
            }
        }
        free(current); 
        free(fullPath);
    }
    return ret;
}