#include "copy.h"

int mmapCopyFile(char *sourceFile, char *destinationFile)
{
    int source, dest;
    struct stat sb;
    size_t fileSize;
    int *sc, *dc;
    int ret;
    
    // If error occurred during opening a source file, return -1.
    if((source = open(sourceFile, O_RDONLY)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: Error occurred during opening %s", sourceFile, destinationFile, sourceFile);
        return -1;
    }

    fstat(source, &sb);
    fileSize = sb.st_size;

    //If error occurred during opening a destination file, return -1.
    if((dest = open(destinationFile, O_CREAT | O_RDWR | O_TRUNC, sb.st_mode)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: Error occurred during opening %s", sourceFile, destinationFile, destinationFile);
        return -1;   
    }

    //If error occurred during mapping a source file to the memory, return -1.
    if((sc = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, source, 0)) == MAP_FAILED)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: Error occurred during mapping %s", sourceFile, destinationFile, sourceFile);
        return -1;
    }

    if(ftruncate(dest, fileSize) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: Error occurred during truncating %s", sourceFile, destinationFile, destinationFile);
        return -1;
    }

    //If error occurred during mapping a destination file to the memory, return -1.
    if((dc = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, dest, 0)) == MAP_FAILED)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: Error occurred during mapping %s", sourceFile, destinationFile, destinationFile);
        return -1;
    }

    //If error occurred during copying files, return -1.
    if(memcpy(dc, sc, fileSize) == NULL)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: Error occurred during copying", sourceFile, destinationFile);
        return -1;
    }

    munmap(sc, fileSize);
    munmap(dc, fileSize);

    close(source);
    close(dest);

    syslog(LOG_INFO, "Copying %s to %s succeed", sourceFile, destinationFile);
    return fileSize;
}

int standardCopyFile(char *sourceFile, char *destinationFile)
{
    int source, destination;
    size_t bytesRead, bytesWritten;
    int fileSize = 0;
    char *buffer;
    struct stat sb;

    //If an error occurred during opening source file, return -1.
    if((source = open(sourceFile, O_RDONLY)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: Error occurred during opening %s", sourceFile, destinationFile, sourceFile);
        return -1;
    }

    fstat(source, &sb);

    //If an error occurred during opening destination file, return -1.
    if((destination = open(destinationFile, O_CREAT | O_WRONLY | O_TRUNC, sb.st_mode)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: Error occurred during opening %s", sourceFile, destinationFile, destinationFile);
        return -1;
    }

    buffer = malloc(sizeof(char) * 131072);

    while((bytesRead = read(source, buffer, 131072)) != 0){
        if(bytesRead == -1)
        {
            if(errno == EINTR)
            {
                continue;
            }
            syslog(LOG_ERR, "Copying %s to %s failed: Error occurred during reading %s", sourceFile, destinationFile, sourceFile);
            break;
        }

        bytesWritten = write(destination, buffer, bytesRead);

        if(bytesWritten == -1){
            if(errno == EINTR)
            {
                continue;
            }
            syslog(LOG_ERR, "Copying %s to %s failed: Error occurred during writing %s", sourceFile, destinationFile, destinationFile);
            break;
        }

        if(bytesWritten != bytesRead)
        {
            syslog(LOG_ERR, "Copying %s to %s failed: Error occurred during copying", sourceFile, destinationFile);
            return -1;
        }

        fileSize += bytesRead;
    }

    close(source);
    close(destination);
    free(buffer);

    syslog(LOG_INFO, "Copying %s to %s succeed.", sourceFile, destinationFile);
    return fileSize;
}

int copyFile(char *_sourcePath, char *_destPath, char *fileName, int sizeTh)
{
    struct stat sb;
    char *sourcePath = mergeStrings(_sourcePath, fileName);
    char *destPath = mergeStrings(_destPath, fileName);

    stat(sourcePath, &sb);

    //If file's size is larger than sizeTh, use mmap copy.
    if(sb.st_size >= sizeTh)
    {
        if((mmapCopyFile(sourcePath, destPath)) != -1)
        {
            syncFilesDate(sourcePath, destPath);
            return 0;
        }
    }
    else
    {
        if((standardCopyFile(sourcePath, destPath)) != -1)
        {
            syncFilesDate(sourcePath, destPath);
            return 0;
        }
    }
}

int copyDirectory(char *_sourceDirectoryPath, char* _destinationDirectoryPath, char *directoryName, int sizeTh)
{
    int destDir;
    int ret;
    struct dirent *entry;
    struct stat sb;
    DIR *source;

    char *sourceDirectoryPath = mergeStrings(_sourceDirectoryPath, directoryName);
    char *destinationDirectoryPath = mergeStrings(_destinationDirectoryPath, directoryName);
    
    stat(sourceDirectoryPath, &sb);

    if((destDir = mkdir(destinationDirectoryPath, sb.st_mode)) == -1)
    {
        return -1;
    }

    ret = 0;

    source = opendir(sourceDirectoryPath);

    while((entry = readdir(source)) != NULL)
    {
        //If current element is a file
        if(entry->d_type == 8)
        {
            if((copyFile(sourceDirectoryPath, destinationDirectoryPath, entry->d_name, sizeTh)) == -1)
            {
                ret = -1;
            }
        }
        //If current element is a directory
        else if(entry->d_type == 4)
        {
            copyDirectory(sourceDirectoryPath, destinationDirectoryPath, entry->d_name, sizeTh);
        }
    }

    syncFilesDate(sourceDirectoryPath, destinationDirectoryPath);

    if(ret == 0)
    {
        syslog(LOG_INFO, "Copying directory %s succeed.", sourceDirectoryPath);

    }
    else
    {
        syslog(LOG_ERR, "Copying directory %s failed: Eror occured during copying content of directory.", sourceDirectoryPath);    
    }
    return ret;
}
