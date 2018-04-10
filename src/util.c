#include "util.h"

char *mergeStrings(char *string1, char *string2)
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
		return -1;
	}
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


int removeWholeList(char *path, List * list)
{
    Node *current;
    char* fullPath;
    int ret = 0;
    while(list->head != NULL)
    {
        current = popElement(list);
        fullPath= mergeStrings(path, current->fileName);
        if(current->fileType != 4)
        {
            remove(fullPath);
        }
        else
        {
            DIR *next = opendir(fullPath);
            List * subList = emptylist();
            loadData(subList, next);
            ret= removeWholeList(fullPath, subList);
            remove(fullPath);
            destroy(subList);
            closedir(next);
        } 
    }
    
    return ret;
}