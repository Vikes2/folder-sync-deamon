#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <utime.h>
#include <dirent.h>
#include <fcntl.h>
#include <syslog.h>
#include "list.h"

/** 
 * [joinToPath]
 * @brief Merges two strings into a correct path, e.g. example/path.
 * @param  stringA:char* the first part of the path
 * @param  stringB:char* the second part of the path which be located after "/"
 * @retval char* joined path
 */
char *joinToPath(char *stringA, char *stringB);

/** 
 * [syncFilesDate]
 * @brief  Updates modify time of a destination path to check the file has been synchronized.  
 * @param  sourcePath:char *
 * @param  destPath: char *
 * @retval 0 - if the update has succeed, 1 - if an error has occurred.
 */
int syncFilesDate(char *sourcePath, char *destPath);

/** 
 * [loadData]
 * @brief  Loads content of given directory to the specified list.
 * @note While it's adding elements, the list contains names and types of elements (file/directory).
 * @param  list: List *
 * @param  dir: DIR *
 * @retval None
 */
void loadData(List *list, DIR *dir);

/** 
 * [removeWholeList]
 * @brief Removes all files and directories from the specified path stored in the list.
 * @param  path: char * 
 * @param  list: List *
 * @retval 0 - if no errors found, 1 - if an error has occurred.
 */
int removeWholeList(char *path, List * list);

#endif