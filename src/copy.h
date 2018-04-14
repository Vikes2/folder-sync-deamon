#ifndef COPY_H
#define COPY_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/mman.h>
#include <syslog.h>
#include "util.h"

/** 
 * [mmapCopyFile]
 * @brief  Copies the file to the destination path using mmap method.
 * @param  sourceFile: char * full path to a source file
 * @param  destinationFile: char* full path to a destination file 
 * @retval -1 - if an error occurred, either size of a copying file.
 */
int mmapCopyFile(char *sourceFile, char *destinationFile);

/** 
 * [standardCopyFile]
 * @brief  Copies the file to the destination path using read and write functions
 * @param  sourceFile: char * full path to a source file 
 * @param  destinationFile: char * full path to a destination file
 * @retval -1 - if an error occurred, either size of a copying file.
 */
int standardCopyFile(char *sourceFile, char *destinationFile);

/** 
 * [copyFile]
 * @brief  Copies the file to the destination path
 * @note   Copy method depends on given sizeTh parameter, if the file's size is larger or equals value of sizeTh, it uses mmap method.
 * @param  _sourcePath: char *
 * @param  _destPath: char *
 * @param  fileName: char *
 * @param  sizeTh: int
 * @retval -1 - if an error occurred, either size of a copying file.
 */
int copyFile(char *_sourcePath, char *_destPath, char *fileName, int sizeTh);

/** 
 * [copyDirectory]
 * @brief  Copies the directory and its content to the destination path
 * @note   Function works when destination path doesn't contain directories supposed to be copied.
 * @param  _sourceDirectoryPath: char *
 * @param  _destinationDirectoryPath: char * 
 * @param  directoryName: char * 
 * @param  sizeTh: int
 * @retval -1 - if an error occurred
 */
int copyDirectory(char *_sourceDirectoryPath, char *_destinationDirectoryPath, char *directoryName, int sizeTh);

#endif