#include "copy.h"

int mmapCopyFile(char *sourceFile, char *destinationFile)
{
    int source, dest;
    struct stat sb;
    size_t fileSize;
    int *sc, *dc;

    if((source = open(sourceFile, O_RDONLY)) == -1)
    {
        perror(sourceFile);
        return -1;
    }

    fstat(source, &sb);
    fileSize = sb.st_size;

    printf("%ld\n", fileSize);

    if((dest = open(destinationFile, O_CREAT | O_RDWR | O_TRUNC, sb.st_mode)) == -1)
    {
        perror(destinationFile);
        return -1;   
    }

    // fileSize = lseek(source, 0, SEEK_END);
    // lseek(dest, fileSize - 1, SEEK_SET);
    // write(dest, '\0', 1);

    if((sc = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, source, 0)) == MAP_FAILED)
    {
        fprintf(stderr, "Error mapping source file: %s\n", sourceFile);
        return -1;
    }

    if((dc = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, dest, 0)) == MAP_FAILED)
    {
        fprintf(stderr, "Error mapping destination file: %s\n", destinationFile);
        return -1;
    }

    if (ftruncate(dest, fileSize) == -1) {  //  Zmień rozmiar pliku docelowego na rozmiaru pliku źródłowego
        //syslog(LOG_ERR, "ftruncate(): \"%s\" (%s)", destPath, strerror(errno));
        return -1;
    }

    if(memcpy(dc, sc, fileSize) == NULL)
    {
        perror("d");
        return -1;
    }
    munmap(sc, fileSize);
    munmap(dc, fileSize);

    printf("EH");

    return fileSize;
}

int standardCopyFile(char* sourceFile, char*destinationFile)
{
    size_t bytesRead;
    size_t bytesWritten;
    char *buffer;

    int source = open(sourceFile, O_RDONLY);
    int destination = open(destinationFile, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    //printf("test");

    if(!source || !destination)
    {
        return 1;
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
            printf("something went wrong.\n");
            return -1;
        }
    }

    //printf("test");

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

    if(sb.st_size >= sizeTh)
    {
        mmapCopyFile(sourcePath, destPath);
    }
    else
    {
        standardCopyFile(sourcePath, destPath);
    }
}

int copyDirectory(char *_sourceDirectoryPath, char* _destinationDirectoryPath, char *directoryName, int sizeTh)
{
    char *sourceDirectoryPath = mergeStrings(_sourceDirectoryPath, directoryName);
    char *destinationDirectoryPath = mergeStrings(_destinationDirectoryPath, directoryName);
    
    int destDir = mkdir(destinationDirectoryPath, 0777);

    if(destDir == -1)
    {
        rmdir(destinationDirectoryPath);
        destDir = mkdir(destinationDirectoryPath, 0777);
    }

    DIR *source = opendir(sourceDirectoryPath);

    struct dirent *entry;

    while((entry = readdir(source)) != NULL)
    {
        if(entry->d_type == 8){
            copyFile(sourceDirectoryPath, destinationDirectoryPath, entry->d_name, sizeTh);
        }
    }

}