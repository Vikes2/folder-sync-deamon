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