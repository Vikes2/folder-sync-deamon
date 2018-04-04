#include "sync.h"


void loadData(List * list, DIR * dir);
  
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




int syncFiles(char* sourceDirPath, char* destinationDirPath, size_t sizeTH)
{
	DIR *source = opendir(sourceDirPath);
    DIR *dest = opendir(destinationDirPath);

    List * list = emptylist();

    loadData(list, source);
    display(list);
    destroy(list);

    closedir(source);
    closedir(dest);

    return 0;
}

int syncRecursive(char* sourceDirPath, char* destinationDirPath, size_t sizeTH)
{

}