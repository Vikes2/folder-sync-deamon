#include "copy.h"

int mmapCopyFile(char *sourceFile, char *destinationFile)
{
    int source, dest;
    struct stat sb;
    size_t fileSize;
    int *sc, *dc;
    int ret;

    ret = 0;
    
    // If error occurred during opening a source file, return -1.
    if((source = open(sourceFile, O_RDONLY)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed; open(): \"%s\" %s", sourceFile, destinationFile, sourceFile, strerror(errno));
        return -1;
    }

    if((fstat(source, &sb)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed; fstat(): \"%s\" %s", sourceFile, destinationFile, sourceFile, strerror(errno));
        ret = -1;
    }

    fileSize = sb.st_size;

    //If error occurred during opening a destination file, return -1.
    if((dest = open(destinationFile, O_CREAT | O_RDWR | O_TRUNC, sb.st_mode)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: open(): \"%s\" %s", sourceFile, destinationFile, destinationFile, strerror(errno));
        if((close(source)) == -1)
            syslog(LOG_ERR, "Copying %s to %s failed: close(): \"%s\" %s", sourceFile, destinationFile, sourceFile, strerror(errno));
        return -1;
    }

    //If error occurred during mapping a source file to the memory, return -1.
    if((sc = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, source, 0)) == MAP_FAILED)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: mmap(): \"%s\" %s", sourceFile, destinationFile, sourceFile, strerror(errno));
        ret = -1;
    }

    if(ftruncate(dest, fileSize) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: ftruncate(): \"%s\" %s", sourceFile, destinationFile, destinationFile, strerror(errno));
        ret = -1;
    }

    //If error occurred during mapping a destination file to the memory, return -1.
    if((dc = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, dest, 0)) == MAP_FAILED)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: mmap(): \"%s\" %s", sourceFile, destinationFile, destinationFile, strerror(errno));
        ret = -1;
    }

    //If error occurred during copying files, return -1.
    if(memcpy(dc, sc, fileSize) == NULL)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: memcpy(): \"%s\" %s", sourceFile, destinationFile, sourceFile, strerror(errno));
        ret = -1;
    }

    //If error occurred during unmapping a source file, return -1.
    if((munmap(sc, fileSize)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: munmap(): \"%s\" %s", sourceFile, destinationFile, sourceFile, strerror(errno));
        ret = -1;
    }

    //If error occurred during unmapping a destination file, return -1.
    if((munmap(dc, fileSize)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: munmap(): \"%s\" %s", sourceFile, destinationFile, destinationFile, strerror(errno));
        ret = -1;
    }
 
    //If error occurred during closing a source file, return -1.
    if((close(source)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: close(): \"%s\" %s", sourceFile, destinationFile, sourceFile, strerror(errno));  
        ret = -1; 
    }
    
    //If error occurred during closing a destination file, return -1.
    if((close(source)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed: close(): \"%s\" %s", sourceFile, destinationFile, sourceFile, strerror(errno));  
        ret = -1; 
    }

    ret = fileSize;

    if(ret != -1)
        syslog(LOG_INFO, "Copying %s to %s succeed", sourceFile, destinationFile);
        
    return ret;
}

int standardCopyFile(char *sourceFile, char *destinationFile)
{
    int source, destination;
    size_t bytesRead, bytesWritten;
    int fileSize = 0, ret = 0;
    char *buffer;
    struct stat sb;

    //If an error occurred during opening source file, return -1.
    if((source = open(sourceFile, O_RDONLY)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed; open(): \"%s\" %s", sourceFile, destinationFile, sourceFile, strerror(errno));
        return -1;
    }

    if((fstat(source, &sb)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed; open(): \"%s\" %s", sourceFile, destinationFile, sourceFile, strerror(errno));
        ret = -1;
    }

    //If an error occurred during opening destination file, return -1.
    if((destination = open(destinationFile, O_CREAT | O_WRONLY | O_TRUNC, sb.st_mode)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed; open(): \"%s\" %s", sourceFile, destinationFile, destinationFile, strerror(errno));
        if((close(source)) == -1)
            syslog(LOG_ERR, "Copying %s to %s failed; open(): \"%s\" %s", sourceFile, destinationFile, sourceFile, strerror(errno));
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
            syslog(LOG_ERR, "Copying %s to %s failed; open(): \"%s\" %s", sourceFile, destinationFile, sourceFile, strerror(errno));
            break;
        }

        bytesWritten = write(destination, buffer, bytesRead);

        if(bytesWritten == -1){
            if(errno == EINTR)
            {
                continue;
            }
            syslog(LOG_ERR, "Copying %s to %s failed; open(): \"%s\" %s", sourceFile, destinationFile, destinationFile, strerror(errno));
            break;
        }

        if(bytesWritten != bytesRead)
        {
            syslog(LOG_ERR, "Copying %s to %s failed: Error occurred during copying", sourceFile, destinationFile);
            ret = -1;
        }

        fileSize += bytesRead;
    }

    //If an error occurred during closing a source file, return -1.
    if((close(source)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed; close(): \"%s\" %s", sourceFile, destinationFile, sourceFile, strerror(errno));
        ret = -1;
    }

    //If an error occurred during closing a destination file, return -1.
    if((close(destination)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed; close(): \"%s\" %s", sourceFile, destinationFile, destinationFile, strerror(errno));
        ret = -1;
    }
    free(buffer);

    ret = fileSize;

    if(ret != -1)
        syslog(LOG_INFO, "Copying %s to %s succeed.", sourceFile, destinationFile);
    return ret;
}

int copyFile(char *_sourcePath, char *_destPath, char *fileName, int sizeTh)
{
    struct stat sb;
    char *sourcePath = joinToPath(_sourcePath, fileName);
    char *destPath = joinToPath(_destPath, fileName);
    int fileSize, ret;

    ret = 0;

    if((stat(sourcePath, &sb)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed; stat(): \"%s\" %s", sourcePath, destPath, sourcePath, strerror(errno));
        free(sourcePath);
        free(destPath);
        return -1;
    }

    //If file's size is larger than sizeTh, use mmap copy.
    if(sb.st_size >= sizeTh)
    {
        if((fileSize = mmapCopyFile(sourcePath, destPath)) != -1)
        {
            syncFilesDate(sourcePath, destPath);
            ret = fileSize;
        }
        else ret = -1;
    }
    else
    {
        if((fileSize = standardCopyFile(sourcePath, destPath)) != -1)
        {
            syncFilesDate(sourcePath, destPath);
            ret = fileSize;
        }
        else ret = -1;
    }

    free(sourcePath);
    free(destPath);
    return ret;
}

int copyDirectory(char *_sourceDirectoryPath, char* _destinationDirectoryPath, char *directoryName, int sizeTh)
{
    int destDir;
    int ret;
    struct dirent *entry;
    struct stat sb;
    DIR *source;

    char *sourceDirectoryPath = joinToPath(_sourceDirectoryPath, directoryName);
    char *destinationDirectoryPath = joinToPath(_destinationDirectoryPath, directoryName);
    
    if((stat(sourceDirectoryPath, &sb)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed; stat(): \"%s\" %s", sourceDirectoryPath, destinationDirectoryPath, sourceDirectoryPath, strerror(errno));
        free(sourceDirectoryPath);
        free(destinationDirectoryPath);
        return -1;
    }

    if((destDir = mkdir(destinationDirectoryPath, sb.st_mode)) == -1)
    {
        if(errno != EEXIST)
            syslog(LOG_ERR, "Copying %s to %s failed; mkdir(): \"%s\" %s", sourceDirectoryPath, destinationDirectoryPath, destinationDirectoryPath, strerror(errno));        
        free(sourceDirectoryPath);
        free(destinationDirectoryPath);
        return -1;
    }

    if((source = opendir(sourceDirectoryPath)) == NULL)
    {
        syslog(LOG_ERR, "Copying %s to %s failed; opendir(): \"%s\" %s", sourceDirectoryPath, destinationDirectoryPath, sourceDirectoryPath, strerror(errno));        
        free(sourceDirectoryPath);
        free(destinationDirectoryPath);
        return -1;
    }

    ret = 0;

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
    
    if((closedir(source)) == -1)
    {
        syslog(LOG_ERR, "Copying %s to %s failed; close(): \"%s\" %s", sourceDirectoryPath, destinationDirectoryPath, sourceDirectoryPath, strerror(errno));
        ret = -1;        
    }

    if(ret == 0)
    {
        syslog(LOG_INFO, "Copying directory %s succeed.", sourceDirectoryPath);

    }
    else
    {
        syslog(LOG_ERR, "Copying directory %s failed: Eror occured during copying content of directory.", sourceDirectoryPath);    
    }

    free(sourceDirectoryPath);
    free(destinationDirectoryPath);
    return ret;
}
