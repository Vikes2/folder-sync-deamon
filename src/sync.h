#ifndef SYNC_H
#define SYNC_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include "list.h"
#include "copy.h"

/** 
 * @brief  Checks if both directories contain the same file.
 * @note   Uses file name, file type and modify time to decide if there are differences and what to do with that.
 * @param  sourceDirPath: char *
 * @param  destinationDirPath: char *
 * @param  element: Node *
 * @param  list: List *
 * @retval 0 - if the source directory and the destination directory contain the same file,
 *         -1 - if the source directory doesn't contain the file
 *         1 - if the files are different
 *         2 - if an error has occurred.
 */
int compare(char* sourceDirPath, char* destinationDirPath, Node* element, List * list);

/** 
 * @brief  Synchronize two directories.
 * @note   If a file in the source directory has different size or modify time, then algorithm copies its to the destination directory.
 *         The same action is raised when there's no file in the destination directory. Any changes in the destination directory are
 *         not allowed, therfore the application removes files existing only in the destination directory or replaces them.
 * @param  sourceDirPath: char *
 * @param  destinationDirPath: char *
 * @param  sizeTh: size_t Size threshold, any larger value than this will be copy using mmap copy method.
 * @param  IsRecursive: int 
 * @retval -1 - if an error has occurred, either it returns 0.
 */
int syncFiles(char* sourceDirPath, char* destinationDirPath, size_t sizeTh, int IsRecursive);

#endif