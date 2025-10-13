#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUFFSZ 1024

void hrm(char* srcp){
    int src = open(srcp, O_WRONLY);
    if(src == -1){
        printf("something went wrong while opening the source file: %s\n", strerror(errno));
        exit(errno);
    }
  
    char* buffer = (char*) malloc(BUFFSZ);
    memset(buffer, 0, BUFFSZ);
    size_t fsize = lseek(src, 0, SEEK_END);
    lseek(src, 0, SEEK_SET);
    for(size_t i=0; i<ceil((long double) fsize/BUFFSZ); i++){
        long writtenBytes = write(src, buffer, (size_t)BUFFSZ);
        
        if(writtenBytes == -1){
            printf("something went wrong while rewriting content: %s\n", strerror(errno));
            exit(errno);
        }
    }
    
    free(buffer);
  
    int srcClose = close(src);
    if(srcClose == -1){
        printf("something went wrong while closing the file: %s\n", strerror(errno));
        exit(errno);
    }
    
    int rm = remove(srcp);
    if(rm == -1){
        printf("something went wrong while deleting the file: %s\n", strerror(errno));
        exit(errno);
    }
}

int main(int argc, char* args[]){
    if(argc < 2){
        printf("you need to specify which file to delete\n");
        exit(1);
    }
    
    hrm(args[1]);
    
    return 0;
}
