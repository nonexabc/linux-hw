#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSZ 1024

void fcpy(char* srcp, char* destp){
    int src = open(srcp, O_RDONLY);
    if(src == -1){
        printf("something went wrong while opening source file: %s\n", strerror(errno));
        exit(errno);
    }
  
    char* buffer = (char*) malloc(BUFFSZ);
    int dest = open(destp, O_WRONLY | O_CREAT | O_TRUNC, 0744);

    if(dest == -1){
        printf("something went wrong while opening destination file: %s\n", strerror(errno));
        exit(errno);
    }
    
    while(1){
        long readBytes = read(src, buffer, BUFFSZ);
        if(readBytes == -1){
            printf("something went wrong while reading source file: %s\n", strerror(errno));
            exit(errno);
        }
        if(readBytes == 0){
            break;
        }
        
        long writtenBytes = write(dest, buffer, (size_t)readBytes);
        
        if(writtenBytes == -1){
            printf("something went wrong while writing into destination file: %s\n", strerror(errno));
            exit(errno);
        }
    }
    
    free(buffer);
  
    int srcClose = close(src);
    int destClose = close(dest);
    if(srcClose == -1){
        printf("something went wrong while closing source file: %s\n", strerror(errno));
        exit(errno);
    }
    if(destClose == -1){
        printf("something went wrong while closing destination file: %s\n", strerror(errno));
        exit(errno);
    }
}

int main(int argc, char* args[]){
    if(argc < 3){
        printf("too little arguments, source and destination must be specified\n");
        exit(1);
    }
    
    fcpy(args[1], args[2]);
    
    return 0;
}
