#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSZ 1023

void readFile(char* fp){
    int fd = open(fp, O_RDONLY);
    if(fd == -1){
        printf("something went wrong while opening: %s\n", strerror(errno));
        exit(errno);
    }
  
    char* buffer = (char*) malloc(BUFFSZ + 1);
    
    while(1){
        long readBytes = read(fd, buffer, BUFFSZ);
        if(readBytes == -1){
            printf("something went wrong while reading: %s\n", strerror(errno));
            exit(errno);
        }
        if(readBytes == 0){
            break;
        }
        buffer[readBytes] = '\0';
        printf("%s", buffer);
    }
    
    free(buffer);
  
    int ccode = close(fd);
    if(ccode == -1){
        printf("something went wrong while closing: %s\n", strerror(errno));
        exit(errno);
    }
}

int main(int argc, char* args[]){
    if(argc < 2){
        printf("no file path specified\n");
        return 0;
    }
    readFile(args[1]);
    return 0;
}
