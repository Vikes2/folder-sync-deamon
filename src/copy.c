#include "copy.h"

int mmapCopyFile(char *sourceFile, char *destinationFile)
{
    int source, dest;
    struct stat sb;
    size_t fileSize;
    int *sc, *dc;

    // If error occurred during opening a source file, return -1.
    if((source = open(sourceFile, O_RDONLY)) == -1)
    {
        perror(sourceFile);
        return -1;
    }

    fstat(source, &sb);
    fileSize = sb.st_size;

    //If error occurred during opening a destination file, return -1.
    if((dest = open(destinationFile, O_CREAT | O_RDWR | O_TRUNC, sb.st_mode)) == -1)
    {
        perror(destinationFile);
        return -1;   
    }

    //If error occurred during mapping a source file to the memory, return -1.
    if((sc = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, source, 0)) == MAP_FAILED)
    {
        fprintf(stderr, "Error mapping source file: %s\n", sourceFile);
        return -1;
    }

    if(ftruncate(dest, fileSize) == -1)
    {
        return -1;
    }

    //If error occurred during mapping a destination file to the memory, return -1.
    if((dc = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, dest, 0)) == MAP_FAILED)
    {
        fprintf(stderr, "Error mapping destination file: %s\n", destinationFile);
        return -1;
    }

    //If error occurred during copying files, return -1.
    if(memcpy(dc, sc, fileSize) == NULL)
    {
        perror("d");
        return -1;
    }

    munmap(sc, fileSize);
    munmap(dc, fileSize);

    close(source);
    close(dest);

    return fileSize;
}

int standardCopyFile(char *sourceFile, char *destinationFile)
{
    int source, destination;
    size_t bytesRead, bytesWritten;
    char *buffer;
    struct stat sb;

    //If an error occurred during opening source file, return -1.
    if((source = open(sourceFile, O_RDONLY)) == -1)
    {
        fprintf(stderr, "Error opening source file %s\n", sourceFile);
        return -1;
    }

    fstat(source, &sb);

    //If an error occurred during opening destination file, return -1.
    if((destination = open(destinationFile, O_CREAT | O_WRONLY | O_TRUNC, sb.st_mode)) == -1)
    {
        fprintf(stderr, "Error opening destination file %s\n", destinationFile);
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
            perror("read");
            break;
        }

        bytesWritten = write(destination, buffer, bytesRead);

        if(bytesWritten == -1){
            if(errno == EINTR)
            {
                continue;
            }
            perror("write");
            break;
        }

        if(bytesWritten != bytesRead)
        {
            fprintf(stderr, "Error while copying files %s to %s\n", sourceFile, destinationFile);
            return -1;
        }
    }

    close(source);
    close(destination);
    free(buffer);
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

    source = opendir(sourceDirectoryPath);

    while((entry = readdir(source)) != NULL)
    {
        //If current element is a file
        if(entry->d_type == 8)
        {
            copyFile(sourceDirectoryPath, destinationDirectoryPath, entry->d_name, sizeTh);
        }
        //If current element is a directory
        else if(entry->d_type == 4)
        {
            copyDirectory(sourceDirectoryPath, destinationDirectoryPath, entry->d_name, sizeTh);
        }
    }

    syncFilesDate(sourceDirectoryPath, destinationDirectoryPath);

}
