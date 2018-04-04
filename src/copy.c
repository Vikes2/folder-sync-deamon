#include "copy.h"

int copyFile(char* sourceFile, char*destinationFile)
{
    size_t bytesRead;
    size_t bytesWritten;
    char *buffer;

    int source = open(sourceFile, O_RDONLY);
    int destination = open(destinationFile, O_CREAT | O_WRONLY | O_TRUNC, 0644);

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

    close(source);
    close(destination);
    free(buffer);
}